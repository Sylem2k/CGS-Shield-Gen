// CGS Shield Gen: SERVER hook for ray-based weapons / modded weapons.
// This is an explicit integration API; the engine's vanilla gun pipeline is NOT hooked here.
// For a hitscan weapon call ServerFilterShot BEFORE applying damage to the target.
// For a physical projectile, a server-side trajectory hook must call this when it
// actually crosses the dome; do not call it repeatedly for the same impact.
class CGS_ShieldInterception
{
	static bool ServerFilterShot(vector start, vector end, float damage, int damageKind, out float damageToTarget, out vector shieldHit)
	{
		damageToTarget = damage;
		shieldHit = vector.Zero;
		if (!Replication.IsServer() || damage <= 0)
			return false;

		array<CGS_ShieldGeneratorComponent> shields = {};
		CGS_ShieldGeneratorComponent.GetAll(shields);
		CGS_ShieldGeneratorComponent nearest = null;
		float nearestDistance = 1000000000;
		foreach (CGS_ShieldGeneratorComponent shield : shields)
		{
			if (!shield)
				continue;
			vector point;
			float distance = shield.IntersectProtectedSegment(start, end, damageKind, point);
			if (distance < 0 || distance >= nearestDistance)
				continue;
			nearest = shield;
			nearestDistance = distance;
			shieldHit = point;
		}
		if (!nearest)
			return false;

		damageToTarget = nearest.ServerResolveIncomingDamage(damage, damageKind);
		return true; // Surface crossed. Caller handles shot effect / damage-to-target.
	}
}
