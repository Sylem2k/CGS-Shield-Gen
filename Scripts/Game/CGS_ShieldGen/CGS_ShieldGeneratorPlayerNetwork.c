// All physical interactions are validated on the dedicated server.
modded class SCR_PlayerController
{
	protected int m_iCGS_SGNextInteractionMs;

	void CGS_SG_RequestToggle(RplId generatorId)
	{
		if (Replication.IsServer()) CGS_SG_RpcToggle(generatorId);
		else Rpc(CGS_SG_RpcToggle, generatorId);
	}

	void CGS_SG_RequestRadiusStep(RplId generatorId, int step)
	{
		if (Replication.IsServer()) CGS_SG_RpcRadiusStep(generatorId, step);
		else Rpc(CGS_SG_RpcRadiusStep, generatorId, step);
	}

	protected CGS_ShieldGeneratorComponent CGS_SG_GetNearbyGenerator(RplId generatorId)
	{
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(generatorId));
		if (!rpl) return null;
		IEntity generator = rpl.GetEntity();
		IEntity caller = GetControlledEntity();
		if (!generator || !caller || vector.Distance(caller.GetOrigin(), generator.GetOrigin()) > 4.0)
			return null;
		CGS_ShieldGeneratorComponent comp = CGS_ShieldGeneratorComponent.Cast(generator.FindComponent(CGS_ShieldGeneratorComponent));
		if (!comp || comp.IsDestroyed()) return null;
		return comp;
	}

	protected bool CGS_SG_InteractionReady()
	{
		int nowMs = System.GetTickCount();
		if (nowMs < m_iCGS_SGNextInteractionMs) return false;
		m_iCGS_SGNextInteractionMs = nowMs + 125;
		return true;
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void CGS_SG_RpcToggle(RplId generatorId)
	{
		if (!CGS_SG_InteractionReady()) return;
		CGS_ShieldGeneratorComponent comp = CGS_SG_GetNearbyGenerator(generatorId);
		if (comp && comp.GetPlayerPowerAllowed()) comp.ServerTogglePower();
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void CGS_SG_RpcRadiusStep(RplId generatorId, int step)
	{
		if (step != 5 && step != -5) return;
		if (!CGS_SG_InteractionReady()) return;
		CGS_ShieldGeneratorComponent comp = CGS_SG_GetNearbyGenerator(generatorId);
		if (comp && comp.GetPlayerRadiusAllowed())
			comp.ServerSetRadius(comp.GetRadius() + step);
	}
}
