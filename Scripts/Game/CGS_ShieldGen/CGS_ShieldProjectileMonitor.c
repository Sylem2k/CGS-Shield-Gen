// CGS Shield Generator V3.4: physical rocket/missile best-effort interception.
// Reuses the tested dynamic-entity scanning pattern from the user's Mech Core point defense.
// Limits: vanilla hitscan bullets/lasers are NOT intercepted here; missiles too fast to
// survive two samples, shell class variations, or explosions spawned outside can bypass.
// This class runs server-side only, once per 250ms shared generator tick.
class CGS_ShieldProjectileMonitor
{
    protected static ref CGS_ShieldProjectileMonitor s_pInstance;
    protected ref array<IEntity> m_aCandidates = {};
    protected ref map<IEntity, vector> m_mPreviousPositions = new map<IEntity, vector>();
    protected ref array<typename> m_aProjectileMoveTypes = {};
    protected bool m_bBuiltTypes;

    static void ServerTick(notnull array<CGS_ShieldGeneratorComponent> generators)
    {
        if (!Replication.IsServer() || !GetGame())
            return;
        if (!s_pInstance)
            s_pInstance = new CGS_ShieldProjectileMonitor();
        s_pInstance.Scan(generators);
    }

    protected void BuildTypes()
    {
        if (m_bBuiltTypes)
            return;
        m_bBuiltTypes = true;
        // Match the known moving-warhead component classes from CGS Mech Core.
        array<string> names = {"MissileMoveComponent", "ShellMoveComponent"};
        foreach (string name : names)
        {
            typename moveType = name.ToType();
            if (moveType)
                m_aProjectileMoveTypes.Insert(moveType);
        }
        Print(string.Format("[CGS SHIELD GEN] Physical projectile scanner: %1 movement classes", m_aProjectileMoveTypes.Count()));
    }

    protected bool IsPhysicalWarhead(IEntity entity)
    {
        if (!entity || entity.IsDeleted() || entity.GetParent())
            return false;
        if (ChimeraCharacter.Cast(entity) || Vehicle.Cast(entity))
            return false;
        foreach (typename moveType : m_aProjectileMoveTypes)
        {
            if (entity.FindComponent(moveType))
                return true;
        }
        return false;
    }

    protected bool CollectCandidate(IEntity entity)
    {
        if (IsPhysicalWarhead(entity) && !m_aCandidates.Contains(entity))
            m_aCandidates.Insert(entity);
        return true;
    }

    protected void Scan(notnull array<CGS_ShieldGeneratorComponent> generators)
    {
        BuildTypes();
        if (m_aProjectileMoveTypes.IsEmpty())
            return;
        BaseWorld world = GetGame().GetWorld();
        if (!world)
            return;
        m_aCandidates.Clear();

        bool active;
        foreach (CGS_ShieldGeneratorComponent gen : generators)
        {
            if (!gen || !gen.IsShieldUp() || !gen.GetProjectileInterception() || !gen.GetBlockExplosive())
                continue;
            active = true;
            // Detection padding keeps a moving missile in the query before it crosses.
            // Query dynamic entities only. Not every terrain piece or static prop.
            world.QueryEntitiesBySphere(gen.GetShieldCentre(), gen.GetRadius() + 80, CollectCandidate, null, EQueryEntitiesFlags.DYNAMIC);
        }
        if (!active)
        {
            m_mPreviousPositions.Clear();
            return;
        }

        map<IEntity, vector> currentPositions = new map<IEntity, vector>();
        foreach (IEntity projectile : m_aCandidates)
        {
            if (!projectile || projectile.IsDeleted())
                continue;
            vector current = projectile.GetOrigin();
            vector previous;
            bool havePrevious = m_mPreviousPositions.Find(projectile, previous);
            bool intercepted;
            if (havePrevious && vector.DistanceSq(previous, current) > 0.001)
            {
                CGS_ShieldGeneratorComponent nearest;
                float nearestDist = 10000000;
                foreach (CGS_ShieldGeneratorComponent shield : generators)
                {
                    if (!shield || !shield.IsShieldUp() || !shield.GetProjectileInterception() || !shield.GetBlockExplosive())
                        continue;
                    vector hitPoint;
                    float dist = shield.IntersectProtectedSegment(previous, current, CGS_ShieldDamageKind.EXPLOSIVE, hitPoint);
                    if (dist >= 0 && dist < nearestDist)
                    {
                        nearestDist = dist;
                        nearest = shield;
                    }
                }
                if (nearest && nearest.ServerStopPhysicalProjectile())
                {
                    Print(string.Format("[CGS SHIELD GEN] Incoming physical warhead intercepted; shield HP %1, radius %2", nearest.GetShieldPoints(), nearest.GetRadius()));
                    // Removing without detonating is intentional: prevents splash inside.
                    SCR_EntityHelper.DeleteEntityAndChildren(projectile);
                    intercepted = true;
                }
            }
            if (!intercepted)
                currentPositions.Insert(projectile, current);
        }
        m_mPreviousPositions = currentPositions;
        m_aCandidates.Clear();
    }
}
