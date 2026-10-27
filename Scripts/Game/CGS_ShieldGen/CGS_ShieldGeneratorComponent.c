// CGS Shield Gen - independent generator state, replication and client-only bubble rendering.
// Server-authoritative shield geometry, GM controls and damage rules. V3.4 monitors slow physical rockets/missiles; vanilla bullets still need a weapon/ballistic hook.
[ComponentEditorProps(category: "CGS/Shield Generator", description: "Standalone editable energy shield generator. Requires RplComponent and destructible damage manager.")]
class CGS_ShieldGeneratorComponentClass : ScriptComponentClass
{
	static CGS_ShieldGeneratorComponent FromEditable(Managed item)
	{
		SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(item);
		if (!editable)
			return null;
		IEntity entity = editable.GetOwner();
		if (!entity)
			return null;
		return CGS_ShieldGeneratorComponent.Cast(entity.FindComponent(CGS_ShieldGeneratorComponent));
	}
}

class CGS_ShieldGeneratorComponent : ScriptComponent
{
	[Attribute("false", UIWidgets.CheckBox, "Shield starts powered on", category: "Generator")]
	protected bool m_bPoweredOnSpawn;

	[Attribute("20", UIWidgets.Slider, "Seconds required to activate the shield", "15 30 1", category: "Generator")]
	protected float m_fInitialStartupSeconds;

	[Attribute("20", UIWidgets.Slider, "Seconds required to shut down the shield", "15 30 1", category: "Generator")]
	protected float m_fInitialShutdownSeconds;

	[Attribute("true", UIWidgets.CheckBox, "Players may use the on/off switch", category: "Player Controls")]
	protected bool m_bInitialPlayerPowerAllowed;

	[Attribute("true", UIWidgets.CheckBox, "Players may adjust shield radius", category: "Player Controls")]
	protected bool m_bInitialPlayerRadiusAllowed;

	[Attribute("true", UIWidgets.CheckBox, "Allow shield regeneration", category: "Shield")]
	protected bool m_bInitialRegenEnabled;

	[Attribute("75", UIWidgets.Slider, "Shield radius in metres", "25 150 5", category: "Shield")]
	protected float m_fInitialRadius;

	[Attribute("5000", UIWidgets.Auto, "Shield capacity", category: "Shield")]
	protected float m_fInitialCapacity;

	[Attribute("100", UIWidgets.Auto, "Shield points restored per second", category: "Shield")]
	protected float m_fInitialRegen;

	[Attribute("5", UIWidgets.Auto, "Regen delay after a hit (seconds)", category: "Shield")]
	protected float m_fRegenDelay;

	[Attribute("15", UIWidgets.Auto, "Delay after shield breaks (seconds)", category: "Shield")]
	protected float m_fBrokenDelay;

	[Attribute("0.25", UIWidgets.Slider, "Minimum capacity fraction before broken shield reactivates", "0.05 1 0.05", category: "Shield")]
	protected float m_fReactivationFraction;

	// Mech Core style protection rules, exposed to the generator prefab and to Game Master.
	[Attribute("true", UIWidgets.CheckBox, "Allow bullets and beams to leave the shield", category: "Protections")]
	protected bool m_bInitialAllowOutgoing;

	[Attribute("true", UIWidgets.CheckBox, "Block ballistic / kinetic fire", category: "Protections")]
	protected bool m_bInitialBlockKinetic;

	[Attribute("true", UIWidgets.CheckBox, "Block rockets, missiles and explosions", category: "Protections")]
	protected bool m_bInitialBlockExplosive;

	[Attribute("true", UIWidgets.CheckBox, "Block energy weapons", category: "Protections")]
	protected bool m_bInitialBlockEnergy;

	[Attribute("true", UIWidgets.CheckBox, "Block EMP", category: "Protections")]
	protected bool m_bInitialBlockEMP;

	[Attribute("true", UIWidgets.CheckBox, "Block true/armour-piercing damage", category: "Protections")]
	protected bool m_bInitialBlockTrue;

	[Attribute("true", UIWidgets.CheckBox, "Block unknown/modded damage by default", category: "Protections")]
	protected bool m_bInitialBlockOther;

	[Attribute("0", UIWidgets.Slider, "Fraction of blocked incoming damage allowed through: 0 = fully stopped, 1 = all through", "0 1 0.05", category: "Protections")]
	protected float m_fInitialBleedThrough;

	[Attribute("1", UIWidgets.Auto, "Shield health cost per kinetic damage point", category: "Damage Costs")]
	protected float m_fInitialKineticCost;

	[Attribute("1", UIWidgets.Auto, "Shield health cost per explosive damage point", category: "Damage Costs")]
	protected float m_fInitialExplosiveCost;

	[Attribute("1", UIWidgets.Auto, "Shield health cost per energy damage point", category: "Damage Costs")]
	protected float m_fInitialEnergyCost;

	[Attribute("3", UIWidgets.Auto, "Shield health cost per EMP damage point", category: "Damage Costs")]
	protected float m_fInitialEMPCost;

	[Attribute("1", UIWidgets.Auto, "Shield health cost per true/armour-piercing damage point", category: "Damage Costs")]
	protected float m_fInitialTrueCost;

	[Attribute("1", UIWidgets.Auto, "Shield health cost per unclassified damage point", category: "Damage Costs")]
	protected float m_fInitialOtherCost;

	[Attribute("", UIWidgets.ResourceNamePicker, "Non-colliding visible shield bubble prefab", params: "et", category: "Visuals")]
	protected ResourceName m_sBubblePrefab;

	[Attribute("0", UIWidgets.Auto, "Bubble mesh radius at scale 1. 0 = measure bounds", category: "Visuals")]
	protected float m_fBubbleMeshRadius;

	[Attribute("true", UIWidgets.CheckBox, "Best-effort server interception of physical rockets/missiles. Not hitscan/vanilla bullets.", category: "Protections")]
	protected bool m_bInitialInterceptProjectiles;

	[Attribute("250", UIWidgets.Auto, "Estimated damage per intercepted physical rocket/missile, before explosive cost multiplier. Used because arbitrary projectile damage is not exposed here.", category: "Protections")]
	protected float m_fInitialProjectileDamage;

	[Attribute("0", UIWidgets.Auto, "Height above generator origin for bubble centre", category: "Visuals")]
	protected float m_fBubbleHeight;

	// OFF=0, STARTING=1, ON=2, STOPPING=3.  Server owns transitions.
	[RplProp(onRplName: "OnRepState")]
	protected int m_iPowerState;

	[RplProp()]
	protected int m_iTransitionRemaining;

	[RplProp()]
	protected float m_fStartupSeconds;

	[RplProp()]
	protected float m_fShutdownSeconds;

	[RplProp()]
	protected bool m_bPlayerPowerAllowed;

	[RplProp()]
	protected bool m_bPlayerRadiusAllowed;

	[RplProp()]
	protected bool m_bRegenEnabled;

	[RplProp()]
	protected float m_fRuntimeRegenDelay;

	[RplProp()]
	protected float m_fRuntimeBrokenDelay;

	[RplProp()]
	protected float m_fRuntimeReactivationFraction;

	[RplProp(onRplName: "OnRepState")]
	protected bool m_bPowered;

	[RplProp(onRplName: "OnRepState")]
	protected bool m_bBroken;

	[RplProp(onRplName: "OnRepState")]
	protected bool m_bDestroyed;

	[RplProp(onRplName: "OnRepState")]
	protected float m_fRadius;

	[RplProp()]
	protected float m_fCapacity;

	[RplProp()]
	protected float m_fShield;

	[RplProp()]
	protected float m_fRegen;

	[RplProp()]
	protected bool m_bAllowOutgoing;
	[RplProp()]
	protected bool m_bBlockKinetic;
	[RplProp()]
	protected bool m_bBlockExplosive;
	[RplProp()]
	protected bool m_bBlockEnergy;
	[RplProp()]
	protected bool m_bBlockEMP;
	[RplProp()]
	protected bool m_bBlockTrue;
	[RplProp()]
	protected bool m_bBlockOther;
	[RplProp()]
	protected float m_fBleedThrough;
	[RplProp()]
	protected float m_fKineticCost;
	[RplProp()]
	protected float m_fExplosiveCost;
	[RplProp()]
	protected float m_fEnergyCost;
	[RplProp()]
	protected float m_fEMPCost;
	[RplProp()]
	protected float m_fTrueCost;
	[RplProp()]
	protected float m_fOtherCost;

	[RplProp()]
	protected bool m_bInterceptProjectiles;

	[RplProp()]
	protected float m_fProjectileDamage;

	protected int m_iTransitionEndMs;
	protected float m_fLastDamageMs;
	protected IEntity m_pBubble;
	protected float m_fBubbleRadiusAtCreation;
	protected float m_fBubbleModelRadius;
	protected vector m_vBubbleBoundsCentre;
	protected vector m_vBubbleLastPosition;
	protected int m_iNextBubbleAttemptMs;
	protected bool m_bWarnedBubbleMissing;
	protected bool m_bWarnedBubbleLoad;
	protected bool m_bLoggedBubbleSuccess;

	// Shared heartbeat avoids one continuously running call-queue task per generator.
	protected static ref array<CGS_ShieldGeneratorComponent> s_aGenerators = {};
	protected static int s_iHeartbeatMs;
	protected static int s_iPreviousTickMs;

	static void GetAll(notnull array<CGS_ShieldGeneratorComponent> outGenerators)
	{
		outGenerators.Clear();
		foreach (CGS_ShieldGeneratorComponent generator : s_aGenerators)
		{
			if (generator && generator.GetOwner())
				outGenerators.Insert(generator);
		}
	}

	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		// Always register this component on replicated GM instances, including the local
		// client visual. Some editor/GM contexts report !InPlayMode on a client.
		if (Replication.IsServer())
		{
			m_bPowered = m_bPoweredOnSpawn;
			m_iPowerState = 0;
			if (m_bPowered)
				m_iPowerState = 2;
			m_iTransitionRemaining = 0;
			m_iTransitionEndMs = 0;
			m_fStartupSeconds = Math.Clamp(m_fInitialStartupSeconds, 15, 30);
			m_fShutdownSeconds = Math.Clamp(m_fInitialShutdownSeconds, 15, 30);
			m_bPlayerPowerAllowed = m_bInitialPlayerPowerAllowed;
			m_bPlayerRadiusAllowed = m_bInitialPlayerRadiusAllowed;
			m_bRegenEnabled = m_bInitialRegenEnabled;
			m_fRuntimeRegenDelay = Math.Max(m_fRegenDelay, 0);
			m_fRuntimeBrokenDelay = Math.Max(m_fBrokenDelay, 0);
			m_fRuntimeReactivationFraction = Math.Clamp(m_fReactivationFraction, 0.05, 1);
			m_fRadius = Math.Clamp(m_fInitialRadius, 25, 150);
			m_fCapacity = Math.Max(m_fInitialCapacity, 1);
			m_fShield = m_fCapacity;
			m_fRegen = Math.Max(m_fInitialRegen, 0);
			m_bAllowOutgoing = m_bInitialAllowOutgoing;
			m_bBlockKinetic = m_bInitialBlockKinetic;
			m_bBlockExplosive = m_bInitialBlockExplosive;
			m_bBlockEnergy = m_bInitialBlockEnergy;
			m_bBlockEMP = m_bInitialBlockEMP;
			m_bBlockTrue = m_bInitialBlockTrue;
			m_bBlockOther = m_bInitialBlockOther;
			m_fBleedThrough = Math.Clamp(m_fInitialBleedThrough, 0, 1);
			m_fKineticCost = Math.Max(m_fInitialKineticCost, 0);
			m_fExplosiveCost = Math.Max(m_fInitialExplosiveCost, 0);
			m_fEnergyCost = Math.Max(m_fInitialEnergyCost, 0);
			m_fEMPCost = Math.Max(m_fInitialEMPCost, 0);
			m_fTrueCost = Math.Max(m_fInitialTrueCost, 0);
			m_fOtherCost = Math.Max(m_fInitialOtherCost, 0);
			m_bInterceptProjectiles = m_bInitialInterceptProjectiles;
			m_fProjectileDamage = Math.Clamp(m_fInitialProjectileDamage, 1, 5000);
			m_bBroken = false;
			m_bDestroyed = false;
			Replication.BumpMe();
		}

		if (!s_aGenerators.Contains(this))
			s_aGenerators.Insert(this);
		EnsureSharedTick();

		// Spawn/size the local visual without waiting for a separate power-state event.
		UpdateBubble();
	}

	override void OnDelete(IEntity owner)
	{
		s_aGenerators.RemoveItem(this);
		DeleteBubble();
		super.OnDelete(owner);
	}

	protected static void EnsureSharedTick()
	{
		if (!GetGame())
			return;
		if (s_iHeartbeatMs != 0 && System.GetTickCount() - s_iHeartbeatMs < 2000)
			return;
		ScriptCallQueue queue = GetGame().GetCallqueue();
		if (!queue)
			return;
		queue.Remove(SharedTick);
		queue.CallLater(SharedTick, 250, true);
		s_iHeartbeatMs = System.GetTickCount();
		s_iPreviousTickMs = 0;
	}

	protected static void SharedTick()
	{
		s_iHeartbeatMs = System.GetTickCount();
		int nowMs = s_iHeartbeatMs;
		float dt = 0;
		if (s_iPreviousTickMs > 0 && nowMs >= s_iPreviousTickMs)
			dt = (nowMs - s_iPreviousTickMs) * 0.001;
		s_iPreviousTickMs = nowMs;

		for (int i = s_aGenerators.Count() - 1; i >= 0; i--)
		{
			CGS_ShieldGeneratorComponent gen = s_aGenerators[i];
			if (!gen || !gen.GetOwner())
			{
				s_aGenerators.Remove(i);
				continue;
			}
			gen.UpdateGenerator(dt, nowMs);
		}

		// Shared scanner: one authoritative pass for all active generators.
		if (Replication.IsServer())
			CGS_ShieldProjectileMonitor.ServerTick(s_aGenerators);
	}

	protected void UpdateGenerator(float dt, int nowMs)
	{
		if (Replication.IsServer())
		{
			SCR_DamageManagerComponent damage = SCR_DamageManagerComponent.Cast(GetOwner().FindComponent(SCR_DamageManagerComponent));
			if (!m_bDestroyed && damage && damage.GetState() == EDamageState.DESTROYED)
			{
				m_bDestroyed = true;
				m_bPowered = false;
				m_iPowerState = 0;
				m_iTransitionEndMs = 0;
				m_iTransitionRemaining = 0;
				m_fShield = 0;
				Replication.BumpMe();
			}

			// Transitions complete only on the server. The dome remains down during STARTING
			// and remains up during STOPPING until the shutdown timer finishes.
			if (!m_bDestroyed && (m_iPowerState == 1 || m_iPowerState == 3))
			{
				if (nowMs >= m_iTransitionEndMs)
				{
					m_bPowered = (m_iPowerState == 1);
					if (m_bPowered)
						m_iPowerState = 2;
					else
						m_iPowerState = 0;
					m_iTransitionRemaining = 0;
					Print(string.Format("[CGS SHIELD GEN] Transition complete: state=%1, powered=%2, shieldHP=%3, radius=%4", m_iPowerState, m_bPowered, m_fShield, GetRadius()));
					Replication.BumpMe();
				}
				else
				{
					int remaining = Math.Ceil((m_iTransitionEndMs - nowMs) * 0.001);
					if (remaining != m_iTransitionRemaining)
					{
						m_iTransitionRemaining = remaining;
						Replication.BumpMe();
					}
				}
			}

			if (!m_bDestroyed && m_bPowered && m_bRegenEnabled && m_fRegen > 0 && dt > 0)
			{
				float delayMs = m_fRuntimeRegenDelay * 1000;
				if (m_bBroken)
					delayMs = m_fRuntimeBrokenDelay * 1000;
				if (nowMs - m_fLastDamageMs >= delayMs && m_fShield < m_fCapacity)
				{
					float before = m_fShield;
					m_fShield = Math.Min(m_fShield + m_fRegen * dt, m_fCapacity);
					if (m_bBroken && m_fShield >= m_fCapacity * Math.Clamp(m_fRuntimeReactivationFraction, 0.05, 1))
						m_bBroken = false;
					// Avoid a network update every frame across large numbers of generators.
					if ((!m_bBroken && before == 0) || nowMs % 1000 < 250 || m_fShield == m_fCapacity)
						Replication.BumpMe();
				}
			}
		}
		UpdateBubble();
	}

	protected void OnRepState()
	{
		UpdateBubble();
	}

	bool IsPowered() { return m_bPowered; }
	bool IsPowerRequested() { return m_iPowerState == 1 || m_iPowerState == 2; }
	bool IsTransitioning() { return m_iPowerState == 1 || m_iPowerState == 3; }
	int GetPowerState() { return m_iPowerState; }
	int GetSecondsRemaining() { return m_iTransitionRemaining; }
	bool IsDestroyed() { return m_bDestroyed; }
	bool IsShieldUp() { return m_bPowered && !m_bDestroyed && !m_bBroken && m_fShield > 0; }
	float GetRadius()
	{
		float radius = m_fRadius;
		if (radius < 25)
			radius = m_fInitialRadius;
		if (radius < 25)
			radius = 75;
		return Math.Clamp(radius, 25, 150);
	}
	float GetCapacity() { return m_fCapacity; }
	float GetShieldPoints() { return m_fShield; }
	float GetRegenPerSecond() { return m_fRegen; }
	float GetStartupSeconds() { return m_fStartupSeconds; }
	float GetShutdownSeconds() { return m_fShutdownSeconds; }
	float GetRegenDelay() { return m_fRuntimeRegenDelay; }
	float GetBrokenDelay() { return m_fRuntimeBrokenDelay; }
	float GetReactivationFraction() { return m_fRuntimeReactivationFraction; }
	bool GetRegenEnabled() { return m_bRegenEnabled; }
	bool GetPlayerPowerAllowed() { return m_bPlayerPowerAllowed; }
	bool GetPlayerRadiusAllowed() { return m_bPlayerRadiusAllowed; }

	// Requests are ignored while transitioning, preventing double-click skipping the timer.
	void ServerSetPowered(bool desiredOn)
	{
		if (!Replication.IsServer() || m_bDestroyed || IsTransitioning() || m_bPowered == desiredOn)
			return;
		int nowMs = System.GetTickCount();
		if (desiredOn)
		{
			m_iPowerState = 1;
			m_iTransitionRemaining = Math.Ceil(m_fStartupSeconds);
			m_iTransitionEndMs = nowMs + m_iTransitionRemaining * 1000;
		}
		else
		{
			m_iPowerState = 3;
			m_iTransitionRemaining = Math.Ceil(m_fShutdownSeconds);
			m_iTransitionEndMs = nowMs + m_iTransitionRemaining * 1000;
		}
		Replication.BumpMe();
	}

	void ServerTogglePower()
	{
		ServerSetPowered(!m_bPowered);
	}

	void ServerSetStartupSeconds(float seconds)
	{
		if (!Replication.IsServer()) return;
		m_fStartupSeconds = Math.Clamp(seconds, 15, 30);
		Replication.BumpMe();
	}

	void ServerSetShutdownSeconds(float seconds)
	{
		if (!Replication.IsServer()) return;
		m_fShutdownSeconds = Math.Clamp(seconds, 15, 30);
		Replication.BumpMe();
	}

	void ServerSetRegenDelay(float seconds)
	{
		if (!Replication.IsServer()) return;
		m_fRuntimeRegenDelay = Math.Clamp(seconds, 0, 120);
		Replication.BumpMe();
	}

	void ServerSetBrokenDelay(float seconds)
	{
		if (!Replication.IsServer()) return;
		m_fRuntimeBrokenDelay = Math.Clamp(seconds, 0, 180);
		Replication.BumpMe();
	}

	void ServerSetReactivationFraction(float fraction)
	{
		if (!Replication.IsServer()) return;
		m_fRuntimeReactivationFraction = Math.Clamp(fraction, 0.05, 1);
		Replication.BumpMe();
	}

	void ServerSetRegenEnabled(bool enabled)
	{
		if (!Replication.IsServer()) return;
		m_bRegenEnabled = enabled;
		Replication.BumpMe();
	}

	void ServerSetPlayerPowerAllowed(bool allowed)
	{
		if (!Replication.IsServer()) return;
		m_bPlayerPowerAllowed = allowed;
		Replication.BumpMe();
	}

	void ServerSetPlayerRadiusAllowed(bool allowed)
	{
		if (!Replication.IsServer()) return;
		m_bPlayerRadiusAllowed = allowed;
		Replication.BumpMe();
	}

	void ServerSetRadius(float radius)
	{
		if (!Replication.IsServer() || m_bDestroyed)
			return;
		m_fRadius = Math.Clamp(radius, 25, 150);
		Print(string.Format("[CGS SHIELD GEN] Radius set: %1 m", m_fRadius));
		Replication.BumpMe();
		UpdateBubble();
	}

	void ServerSetCapacity(float capacity)
	{
		if (!Replication.IsServer() || m_bDestroyed)
			return;
		m_fCapacity = Math.Max(capacity, 1);
		m_fShield = Math.Min(m_fShield, m_fCapacity);
		Replication.BumpMe();
	}

	void ServerSetRegen(float regen)
	{
		if (!Replication.IsServer() || m_bDestroyed)
			return;
		m_fRegen = Math.Max(regen, 0);
		Replication.BumpMe();
	}

	// Protection IDs deliberately do not depend on Mech Core enums or other mod classes.
	// Use CGS_ShieldDamageKind.KINETIC / EXPLOSIVE / ENERGY / EMP / OTHER.
	bool IsDamageTypeBlocked(int kind)
	{
		switch (kind)
		{
			case CGS_ShieldDamageKind.KINETIC: return m_bBlockKinetic;
			case CGS_ShieldDamageKind.EXPLOSIVE: return m_bBlockExplosive;
			case CGS_ShieldDamageKind.ENERGY: return m_bBlockEnergy;
			case CGS_ShieldDamageKind.EMP: return m_bBlockEMP;
			case CGS_ShieldDamageKind.TRUE: return m_bBlockTrue;
		}
		return m_bBlockOther;
	}

	float GetDamageCost(int kind)
	{
		switch (kind)
		{
			case CGS_ShieldDamageKind.KINETIC: return m_fKineticCost;
			case CGS_ShieldDamageKind.EXPLOSIVE: return m_fExplosiveCost;
			case CGS_ShieldDamageKind.ENERGY: return m_fEnergyCost;
			case CGS_ShieldDamageKind.EMP: return m_fEMPCost;
			case CGS_ShieldDamageKind.TRUE: return m_fTrueCost;
		}
		return m_fOtherCost;
	}

	bool GetAllowOutgoing() { return m_bAllowOutgoing; }
	float GetBleedThrough() { return m_fBleedThrough; }
	bool GetBlockKinetic() { return m_bBlockKinetic; }
	bool GetBlockExplosive() { return m_bBlockExplosive; }
	bool GetBlockEnergy() { return m_bBlockEnergy; }
	bool GetBlockEMP() { return m_bBlockEMP; }
	bool GetBlockTrue() { return m_bBlockTrue; }
	bool GetBlockOther() { return m_bBlockOther; }
	float GetKineticCost() { return m_fKineticCost; }
	float GetExplosiveCost() { return m_fExplosiveCost; }
	float GetEnergyCost() { return m_fEnergyCost; }
	float GetEMPCost() { return m_fEMPCost; }
	float GetTrueCost() { return m_fTrueCost; }
	float GetOtherCost() { return m_fOtherCost; }
	bool GetProjectileInterception() { return m_bInterceptProjectiles; }
	float GetProjectileDamage() { return m_fProjectileDamage; }

	void ServerSetAllowOutgoing(bool enabled) { if (!Replication.IsServer()) return; m_bAllowOutgoing = enabled; Replication.BumpMe(); }
	void ServerSetBlockKinetic(bool enabled) { if (!Replication.IsServer()) return; m_bBlockKinetic = enabled; Replication.BumpMe(); }
	void ServerSetBlockExplosive(bool enabled) { if (!Replication.IsServer()) return; m_bBlockExplosive = enabled; Replication.BumpMe(); }
	void ServerSetBlockEnergy(bool enabled) { if (!Replication.IsServer()) return; m_bBlockEnergy = enabled; Replication.BumpMe(); }
	void ServerSetBlockEMP(bool enabled) { if (!Replication.IsServer()) return; m_bBlockEMP = enabled; Replication.BumpMe(); }
	void ServerSetBlockTrue(bool enabled) { if (!Replication.IsServer()) return; m_bBlockTrue = enabled; Replication.BumpMe(); }
	void ServerSetBlockOther(bool enabled) { if (!Replication.IsServer()) return; m_bBlockOther = enabled; Replication.BumpMe(); }
	void ServerSetBleedThrough(float fraction) { if (!Replication.IsServer()) return; m_fBleedThrough = Math.Clamp(fraction, 0, 1); Replication.BumpMe(); }
	void ServerSetKineticCost(float cost) { if (!Replication.IsServer()) return; m_fKineticCost = Math.Max(cost, 0); Replication.BumpMe(); }
	void ServerSetExplosiveCost(float cost) { if (!Replication.IsServer()) return; m_fExplosiveCost = Math.Max(cost, 0); Replication.BumpMe(); }
	void ServerSetEnergyCost(float cost) { if (!Replication.IsServer()) return; m_fEnergyCost = Math.Max(cost, 0); Replication.BumpMe(); }
	void ServerSetEMPCost(float cost) { if (!Replication.IsServer()) return; m_fEMPCost = Math.Max(cost, 0); Replication.BumpMe(); }
	void ServerSetTrueCost(float cost) { if (!Replication.IsServer()) return; m_fTrueCost = Math.Max(cost, 0); Replication.BumpMe(); }
	void ServerSetOtherCost(float cost) { if (!Replication.IsServer()) return; m_fOtherCost = Math.Max(cost, 0); Replication.BumpMe(); }
	void ServerSetProjectileInterception(bool enabled) { if (!Replication.IsServer()) return; m_bInterceptProjectiles = enabled; Replication.BumpMe(); }
	void ServerSetProjectileDamage(float damage) { if (!Replication.IsServer()) return; m_fProjectileDamage = Math.Clamp(damage, 1, 5000); Replication.BumpMe(); }

	vector GetShieldCentre()
	{
		IEntity owner = GetOwner();
		if (!owner)
			return vector.Zero;
		return owner.GetOrigin() + vector.Up * m_fBubbleHeight;
	}

	// A segment must actually cross the protective surface. Inside -> outside is
	// free by default; when outward blocking is disabled nobody pays shield HP.
	// Geometry matches the CURRENT FULL SPHERE visual (not a hemisphere).
	// Returns distance from segment start to the first eligible surface crossing.
	float IntersectProtectedSegment(vector start, vector end, int damageKind, out vector surface)
	{
		surface = vector.Zero;
		if (!IsShieldUp() || !IsDamageTypeBlocked(damageKind))
			return -1;

		vector delta = end - start;
		float range = delta.Length();
		if (range < 0.001)
			return -1;
		vector direction = delta / range;
		vector center = GetShieldCentre();
		float radius = GetRadius();
		vector fromCentre = start - center;
		float c = vector.Dot(fromCentre, fromCentre) - radius * radius;
		bool startsInside = c < -0.01;
		if (startsInside && m_bAllowOutgoing)
			return -1;

		float b = vector.Dot(fromCentre, direction);
		float discriminant = b * b - c;
		if (discriminant < 0)
			return -1;
		float root = Math.Sqrt(discriminant);
		float distance = -b - root;
		if (startsInside)
			distance = -b + root;
		if (distance < 0 || distance > range)
			return -1;
		if (!startsInside && b >= 0)
			return -1;

		surface = start + direction * distance;
		return distance;
	}

	// [SERVER] Only processes a hit after the caller has found the crossing.
	// Returns the amount of incoming DAMAGE remaining after shield absorption.
	// Weapon integrations must apply this remainder themselves; this does not
	// magically intercept vanilla bullet simulation or HandleDamage calls.
	float ServerResolveIncomingDamage(float damage, int damageKind)
	{
		if (!Replication.IsServer() || !IsShieldUp() || damage <= 0 || !IsDamageTypeBlocked(damageKind))
			return damage;

		float bleed = Math.Clamp(m_fBleedThrough, 0, 1);
		float blockedDamage = damage * (1 - bleed);
		float costFactor = Math.Max(GetDamageCost(damageKind), 0);
		float cost = blockedDamage * costFactor;
		float passedDamage = damage * bleed;
		if (cost > m_fShield && costFactor > 0)
			passedDamage += (cost - m_fShield) / costFactor;
		float consumed = Math.Min(cost, m_fShield);
		m_fShield -= consumed;
		m_fLastDamageMs = System.GetTickCount();
		if (m_fShield <= 0 && cost > 0)
		{
			m_fShield = 0;
			m_bBroken = true;
		}
		Replication.BumpMe();
		UpdateBubble();
		return passedDamage;
	}

	// [SERVER] A complete physical-warhead stop (rocket/missile).
	// The engine projectile cannot be reliably resized to reflect partial damage,
	// so bleed-through > 0 means this best-effort monitor leaves it untouched.
	// A shield with insufficient HP is depleted and lets the whole warhead pass.
	bool ServerStopPhysicalProjectile()
	{
		if (!Replication.IsServer() || !IsShieldUp() || !m_bInterceptProjectiles || !m_bBlockExplosive)
			return false;
		if (m_fBleedThrough > 0.001)
			return false;

		float cost = Math.Max(m_fProjectileDamage, 1) * Math.Max(m_fExplosiveCost, 0);
		bool stopped = m_fShield >= cost;
		m_fShield = Math.Max(0, m_fShield - cost);
		m_fLastDamageMs = System.GetTickCount();
		if (m_fShield <= 0)
		{
			m_fShield = 0;
			m_bBroken = true;
		}
		Replication.BumpMe();
		UpdateBubble();
		return stopped;
	}

	// Called by future dedicated weapon interceptors. Does NOT intercept vanilla shots by itself.
	float ServerAbsorb(float amount)
	{
		if (!Replication.IsServer() || !IsShieldUp() || amount <= 0)
			return 0;
		float absorbed = Math.Min(amount, m_fShield);
		m_fShield -= absorbed;
		m_fLastDamageMs = System.GetTickCount();
		if (m_fShield <= 0)
		{
			m_fShield = 0;
			m_bBroken = true;
		}
		Replication.BumpMe();
		UpdateBubble();
		return absorbed;
	}

	// The visual bubble is a client-local mesh. It is intentionally NOT a second
	// MeshObject on the generator or an independently networked/physical object.
	// The protected radius is read from the generator's replicated m_fRadius.
	protected void UpdateBubble()
	{
		if (System.IsConsoleApp() || !GetOwner())
			return;

		if (!m_pBubble)
		{
			int nowMs = System.GetTickCount();
			if (nowMs < m_iNextBubbleAttemptMs)
				return;
			m_iNextBubbleAttemptMs = nowMs + 5000;
			CreateBubble();
		}

		if (!m_pBubble)
			return;

		float radius = GetRadius();
		if (Math.AbsFloat(m_fBubbleRadiusAtCreation - radius) > 0.01 || vector.Distance(m_vBubbleLastPosition, GetOwner().GetOrigin()) > 0.05)
			ResizeBubble();

		// Clients can receive powered state before the replicated HP value. Broken
		// is the authoritative collapse flag; don't hide an otherwise active
		// bubble just because shield HP is temporarily 0 on a new client.
		if (m_bPowered && !m_bDestroyed && !m_bBroken)
			m_pBubble.SetFlags(EntityFlags.VISIBLE, true);
		else
			m_pBubble.ClearFlags(EntityFlags.VISIBLE, true);
	}

	protected void CreateBubble()
	{
		if (m_sBubblePrefab.IsEmpty())
		{
			if (!m_bWarnedBubbleMissing)
			{
				Print("[CGS SHIELD GEN] Bubble Prefab is empty on generator. Set m_sBubblePrefab in CGS_ShieldGenerator.et", LogLevel.WARNING);
				m_bWarnedBubbleMissing = true;
			}
			return;
		}

		Resource resource = Resource.Load(m_sBubblePrefab);
		if (!resource || !resource.IsValid())
		{
			if (!m_bWarnedBubbleLoad)
			{
				Print(string.Format("[CGS SHIELD GEN] FAILED to load bubble prefab: %1", m_sBubblePrefab), LogLevel.WARNING);
				m_bWarnedBubbleLoad = true;
			}
			return;
		}

		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		Math3D.MatrixIdentity4(params.Transform);
		params.Transform[3] = GetShieldCentre();
		m_pBubble = GetGame().SpawnEntityPrefabLocal(resource, GetOwner().GetWorld(), params);
		if (!m_pBubble)
		{
			if (!m_bWarnedBubbleLoad)
			{
				Print("[CGS SHIELD GEN] FAILED to spawn local bubble entity", LogLevel.WARNING);
				m_bWarnedBubbleLoad = true;
			}
			return;
		}

		vector mins, maxs;
		m_pBubble.GetBounds(mins, maxs);
		m_vBubbleBoundsCentre = (mins + maxs) * 0.5;
		vector half = (maxs - mins) * 0.5;
		float measured = Math.Max(half[0], Math.Max(half[1], half[2]));
		m_fBubbleModelRadius = m_fBubbleMeshRadius;
		if (m_fBubbleModelRadius <= 0)
		{
			m_fBubbleModelRadius = measured;
			// A huge or zero result means GetBounds was not usable for this mesh.
			if (m_fBubbleModelRadius < 0.01 || m_fBubbleModelRadius > 20)
			{
				Print(string.Format("[CGS SHIELD GEN] Unusable bubble mesh bounds (radius %1). Falling back to 1 m; set Bubble mesh radius in prefab if needed.", measured), LogLevel.WARNING);
				m_fBubbleModelRadius = 1;
				m_vBubbleBoundsCentre = vector.Zero;
			}
		}
		if (m_fBubbleModelRadius < 0.01)
			m_fBubbleModelRadius = 1;

		// Force the initial scale even when the replicated radius has not changed yet.
		m_fBubbleRadiusAtCreation = -1;
		ResizeBubble();

		if (!m_bLoggedBubbleSuccess)
		{
			Print(string.Format("[CGS SHIELD GEN] Bubble spawned: radius %1 m, model radius %2 m, scale %3", GetRadius(), m_fBubbleModelRadius, GetRadius() / m_fBubbleModelRadius));
			m_bLoggedBubbleSuccess = true;
		}
	}

	protected void ResizeBubble()
	{
		if (!m_pBubble || !GetOwner())
			return;

		// IMPORTANT: IEntity.SetWorldTransform() is for positioning/orientation.
		// Enfusion does not reliably preserve uniform scale encoded in its basis
		// vectors. Use IEntity.SetScale() (also used by our Mech Core debris).
		float radius = GetRadius();
		float modelRadius = Math.Max(m_fBubbleModelRadius, 0.01);
		float scale = radius / modelRadius;

		vector world[4];
		Math3D.MatrixIdentity4(world);
		// The shield sphere is centred at the mesh's origin. Offset by local
		// bounding centre for non-centred replacement meshes.
		world[3] = GetShieldCentre() - m_vBubbleBoundsCentre * scale;
		m_pBubble.SetWorldTransform(world);
		m_pBubble.SetScale(scale);

		// Only called on initial create, radius change, or generator movement.
		// This confirms what scale Enfusion actually applied on the visual.
		if (Math.AbsFloat(m_fBubbleRadiusAtCreation - radius) > 0.01)
		{
			Print(string.Format("[CGS SHIELD GEN] Bubble resize: radius=%1 m, source mesh radius=%2, scale requested=%3, applied=%4", radius, modelRadius, scale, m_pBubble.GetScale()));
		}
		m_fBubbleRadiusAtCreation = radius;
		m_vBubbleLastPosition = GetOwner().GetOrigin();
	}

	protected void DeleteBubble()
	{
		if (!m_pBubble)
			return;
		SCR_EntityHelper.DeleteEntityAndChildren(m_pBubble);
		m_pBubble = null;
	}
}
