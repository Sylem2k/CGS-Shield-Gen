// Game Master attribute classes. Read returns null on non-generator entities.

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_PowerEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.IsPowerRequested());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetPowered(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_RadiusEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetRadius());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetRadius(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_CapacityEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetCapacity());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetCapacity(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_RegenEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetRegenPerSecond());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetRegen(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_RegenEnabledEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetRegenEnabled());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetRegenEnabled(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_RegenDelayEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetRegenDelay());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetRegenDelay(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_BrokenDelayEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetBrokenDelay());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetBrokenDelay(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_ReactivationFractionEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetReactivationFraction());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetReactivationFraction(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_StartupSecondsEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetStartupSeconds());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetStartupSeconds(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_ShutdownSecondsEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetShutdownSeconds());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetShutdownSeconds(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_PlayerPowerEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetPlayerPowerAllowed());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetPlayerPowerAllowed(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_PlayerRadiusEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetPlayerRadiusAllowed());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetPlayerRadiusAllowed(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_OutgoingEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetAllowOutgoing());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetAllowOutgoing(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_KineticEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetBlockKinetic());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetBlockKinetic(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_ExplosiveEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetBlockExplosive());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetBlockExplosive(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_EnergyEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetBlockEnergy());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetBlockEnergy(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_EMPEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetBlockEMP());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetBlockEMP(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_OtherEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetBlockOther());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetBlockOther(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_BleedEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetBleedThrough());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetBleedThrough(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_KineticCostEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetKineticCost());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetKineticCost(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_ExplosiveCostEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetExplosiveCost());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetExplosiveCost(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_EnergyCostEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetEnergyCost());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetEnergyCost(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_EMPCostEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetEMPCost());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetEMPCost(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_OtherCostEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetOtherCost());
	}

	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetOtherCost(var.GetFloat());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_TrueEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateBool(gen.GetBlockTrue());
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetBlockTrue(var.GetBool());
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_TrueCostEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (!gen) return null;
		return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetTrueCost());
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
		if (gen && var) gen.ServerSetTrueCost(var.GetFloat());
	}
}

// CGS V3.4 physical rocket/missile settings (do not affect vanilla bullet intercept).
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_InterceptPhysicalEditorAttribute : SCR_BaseValueListEditorAttribute
{
    override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
    {
        CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
        if (!gen) return null;
        return SCR_BaseEditorAttributeVar.CreateBool(gen.GetProjectileInterception());
    }

    override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
    {
        CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
        if (gen && var) gen.ServerSetProjectileInterception(var.GetBool());
    }
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class CGS_SG_PhysicalWarheadDamageEditorAttribute : SCR_BaseValueListEditorAttribute
{
    override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
    {
        CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
        if (!gen) return null;
        return SCR_BaseEditorAttributeVar.CreateFloat(gen.GetProjectileDamage());
    }

    override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
    {
        CGS_ShieldGeneratorComponent gen = CGS_ShieldGeneratorComponentClass.FromEditable(item);
        if (gen && var) gen.ServerSetProjectileDamage(var.GetFloat());
    }
}
