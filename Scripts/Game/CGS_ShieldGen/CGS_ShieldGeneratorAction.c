// In-world buttons; the server validates every request from a nearby player.
[BaseContainerProps()]
class CGS_ShieldGeneratorBaseAction : ScriptedUserAction
{
	protected CGS_ShieldGeneratorComponent FindGenerator()
	{
		IEntity owner = GetOwner();
		while (owner && owner.GetParent())
			owner = owner.GetParent();
		if (!owner) return null;
		return CGS_ShieldGeneratorComponent.Cast(owner.FindComponent(CGS_ShieldGeneratorComponent));
	}

	override bool HasLocalEffectOnlyScript() { return true; }

	protected RplId GetGeneratorId(CGS_ShieldGeneratorComponent gen)
	{
		RplComponent rpl = RplComponent.Cast(gen.GetOwner().FindComponent(RplComponent));
		if (!rpl) return RplId.Invalid();
		return rpl.Id();
	}
}

[BaseContainerProps()]
class CGS_ShieldGeneratorAction : CGS_ShieldGeneratorBaseAction
{
	override bool CanBeShownScript(IEntity user)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		return gen && !gen.IsDestroyed() && gen.GetPlayerPowerAllowed();
	}
	override bool CanBePerformedScript(IEntity user)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		return CanBeShownScript(user) && gen && !gen.IsTransitioning();
	}
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!gen || !controller) return;
		controller.CGS_SG_RequestToggle(GetGeneratorId(gen));
	}
	override bool GetActionNameScript(out string outName)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		if (!gen) return false;
		if (gen.GetPowerState() == 1)
			outName = string.Format("SHIELD STARTING (%1s)", gen.GetSecondsRemaining());
		else if (gen.GetPowerState() == 3)
			outName = string.Format("SHIELD SHUTTING DOWN (%1s)", gen.GetSecondsRemaining());
		else if (gen.IsPowered())
			outName = "SHUT DOWN SHIELD";
		else
			outName = "START SHIELD";
		return true;
	}
}

[BaseContainerProps()]
class CGS_ShieldGeneratorRadiusUpAction : CGS_ShieldGeneratorBaseAction
{
	override bool CanBeShownScript(IEntity user)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		return gen && !gen.IsDestroyed() && gen.GetPlayerRadiusAllowed();
	}
	override bool CanBePerformedScript(IEntity user)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		return CanBeShownScript(user) && gen.GetRadius() < 150;
	}
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!gen || !controller) return;
		controller.CGS_SG_RequestRadiusStep(GetGeneratorId(gen), 5);
	}
	override bool GetActionNameScript(out string outName)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		if (!gen) return false;
		outName = string.Format("SHIELD RADIUS +5m (CURRENT: %1m)", gen.GetRadius());
		return true;
	}
}

[BaseContainerProps()]
class CGS_ShieldGeneratorRadiusDownAction : CGS_ShieldGeneratorBaseAction
{
	override bool CanBeShownScript(IEntity user)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		return gen && !gen.IsDestroyed() && gen.GetPlayerRadiusAllowed();
	}
	override bool CanBePerformedScript(IEntity user)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		return CanBeShownScript(user) && gen.GetRadius() > 25;
	}
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!gen || !controller) return;
		controller.CGS_SG_RequestRadiusStep(GetGeneratorId(gen), -5);
	}
	override bool GetActionNameScript(out string outName)
	{
		CGS_ShieldGeneratorComponent gen = FindGenerator();
		if (!gen) return false;
		outName = string.Format("SHIELD RADIUS -5m (CURRENT: %1m)", gen.GetRadius());
		return true;
	}
}
