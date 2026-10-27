// CGS Shield Generator V3.7
// SERVER: shield outgoing/incoming checks for vanilla and compatible scripted damage.
// Use the supported damage-manager HijackDamageHandling extension (super first).
// The projectile monitor handles physical rockets separately. This patch handles
// damage reaching characters / vehicles / props and does not create a collision dome.
//
// Important: source must be a resolvable instigator; never block environmental,
// healing, collision, melee or unowned damage. That prevents blanket invulnerability.
modded class SCR_DamageManagerComponent
{
	//------------------------------------------------------------------------------------------------
	override bool HijackDamageHandling(notnull BaseDamageContext damageContext)
	{
		if (super.HijackDamageHandling(damageContext))
			return true;

		if (!Replication.IsServer() || damageContext.damageValue <= 0 || !damageContext.instigator)
			return false;

		// Classify only positive impact damage types with a known ballistic/impact source.
		// ENERGY / EMP / OTHER are supported by the existing explicit custom-weapon API;
		// we do not reinterpret unrelated vanilla DoT and environment damage as those.
		int kind = -1;
		switch (damageContext.damageType)
		{
			case EDamageType.KINETIC:
				kind = CGS_ShieldDamageKind.KINETIC;
				break;
			case EDamageType.EXPLOSIVE:
			case EDamageType.FRAGMENTATION:
			case EDamageType.PROCESSED_FRAGMENTATION:
				kind = CGS_ShieldDamageKind.EXPLOSIVE;
				break;
			case EDamageType.TRUE:
				kind = CGS_ShieldDamageKind.TRUE;
				break;
			default:
				return false;
		}

		IEntity target = GetOwner();
		if (!target)
			return false;

		IEntity attacker = damageContext.instigator.GetInstigatorEntity();

		// Player-owned weapons can name a carried weapon or another entity as the
		// instigator. Resolve the controlled character when its player ID is known.
		PlayerManager players = GetGame().GetPlayerManager();
		if (players)
		{
			int playerID = damageContext.instigator.GetInstigatorPlayerID();
			if (playerID > 0)
			{
				IEntity controlled = players.GetPlayerControlledEntity(playerID);
				if (controlled)
					attacker = controlled;
			}
		}

		if (!attacker || attacker.IsDeleted())
			return false;

		// For AI guns and mounted weapons, their outermost parent normally provides
		// the vehicle / AI entity's position instead of the weapon's muzzle.
		// Do not replace a resolved player character's origin with its vehicle's origin.
		if (!ChimeraCharacter.Cast(attacker))
		{
			IEntity root = attacker.GetRootParent();
			if (root)
				attacker = root;
		}

		if (attacker == target)
			return false;

		vector start = attacker.GetOrigin();
		vector end = damageContext.hitPosition;

		// Some area-damage contexts do not provide a world impact point.
		// Use the target origin only when the hit point is absent.
		if (vector.DistanceSq(end, vector.Zero) < 0.0001)
			end = target.GetOrigin();

		if (vector.DistanceSq(start, end) < 0.001)
			return false;

		float damageRemaining;
		vector shieldHit;
		if (!CGS_ShieldInterception.ServerFilterShot(
			start, end, damageContext.damageValue, kind, damageRemaining, shieldHit))
			return false;

		// The shield was crossed. ServerFilterShot has already charged shield HP.
		// Returning false applies partial bleed-through / damage in excess of
		// remaining shield HP through the ordinary damage pipeline.
		if (damageRemaining >= damageContext.damageValue)
			return false;

		damageContext.damageValue = Math.Max(damageRemaining, 0);
		if (damageContext.damageValue <= 0.0001)
		{
			damageContext.damageValue = 0;
			return true;
		}
		return false;
	}
}
