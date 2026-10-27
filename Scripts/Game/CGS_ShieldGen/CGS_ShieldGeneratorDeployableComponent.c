// Add this component to a compatible inventory deployable item prefab.
// Uses the same server-only placement and surface trace pattern as BFA Sentinel.
[EntityEditorProps(category: "GameScripted/CGS Shield Generator", description: "Inventory item which deploys a CGS Shield Generator")]
class CGS_ShieldGeneratorDeployableComponentClass : SCR_BaseDeployableInventoryItemComponentClass
{
}

class CGS_ShieldGeneratorDeployableComponent : SCR_BaseDeployableInventoryItemComponent
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Generator prefab to spawn")]
	protected ResourceName m_sGeneratorPrefab;

	[Attribute("3.5", UIWidgets.Auto, "Placement distance in front of player")]
	protected float m_fForwardDistance;

	[Attribute("0.1", UIWidgets.Auto, "Vertical offset from surface")]
	protected float m_fSurfaceOffset;

	[Attribute("0", UIWidgets.Auto, "Additional yaw rotation (degrees)")]
	protected float m_fYawOffset;

	override void Deploy(IEntity userEntity = null, bool reload = false)
	{
		if (!m_RplComponent || m_RplComponent.IsProxy() || m_bIsDeployed || !userEntity)
			return;
		IEntity item = GetOwner();
		if (!item || m_sGeneratorPrefab.IsEmpty())
			return;
		BaseWorld world = item.GetWorld();
		if (!world)
			return;
		Resource resource = Resource.Load(m_sGeneratorPrefab);
		if (!resource || !resource.IsValid())
			return;

		vector playerMat[4];
		userEntity.GetWorldTransform(playerMat);
		vector forward = playerMat[2];
		forward[1] = 0;
		if (forward.LengthSq() < 0.001)
			forward = "0 0 1";
		forward.Normalize();
		vector pos = userEntity.GetOrigin() + forward * Math.Max(m_fForwardDistance, 1.5);

		TraceParam trace = new TraceParam();
		trace.Start = pos + vector.Up * 4.0;
		trace.End = pos - vector.Up * 12.0;
		trace.Flags = TraceFlags.WORLD | TraceFlags.ENTS;
		trace.LayerMask = EPhysicsLayerPresets.Projectile;
		array<IEntity> exclusions = {userEntity, item};
		trace.ExcludeArray = exclusions;
		float hit = world.TraceMove(trace, null);
		if (hit < 1.0)
			pos = trace.Start + (trace.End - trace.Start) * hit;
		else
			pos[1] = world.GetSurfaceY(pos[0], pos[2]);
		pos += vector.Up * m_fSurfaceOffset;

		vector angles = Math3D.MatrixToAngles(playerMat);
		angles[0] = angles[0] + m_fYawOffset;
		angles[1] = 0;
		angles[2] = 0;
		vector spawnMat[4];
		Math3D.AnglesToMatrix(angles, spawnMat);
		spawnMat[3] = pos;

		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform = spawnMat;
		IEntity generator = GetGame().SpawnEntityPrefab(resource, world, params);
		if (!generator)
			return;

		m_bIsDeployed = true;
		SetItemOwner(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(userEntity));
		Replication.BumpMe();
		if (m_OnDeployedStateChanged)
			m_OnDeployedStateChanged.Invoke(true, this);
		SCR_EntityHelper.DeleteEntityAndChildren(item);
	}
}
