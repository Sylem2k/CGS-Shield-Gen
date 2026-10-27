# CGS Shield Gen V3.7 — Incoming impact damage filter (Workbench test build)

## Scope
This is the first engine damage-pipeline integration. It is **not** a physical collider and doesn't stop projectile/tracer graphics at the bubble surface. It intercepts **damage events** on the server via the documented Reforger `SCR_DamageManagerComponent.HijackDamageHandling(BaseDamageContext)` extension hook.

The existing `CGS_ShieldInterception.ServerFilterShot()` finds the first active sphere crossed by a line segment and charges its HP using the user's existing Game Master protection settings:
- **Kinetic** maps to `EDamageType.KINETIC`.
- **Explosive** maps to `EXPLOSIVE`, `FRAGMENTATION`, `PROCESSED_FRAGMENTATION`.
- **True / armor-piercing** maps to `TRUE`.
- **Energy / EMP / Other** still use the explicit `CGS_ShieldInterception` custom-weapon interface. Vanilla doesn't identify those semantic weapon classes for us.
- Collision, healing, melee, environmental effects, periodic fire and damage without an identifiable attacker remain unchanged; no general damage immunity is applied.

The filter uses the instigator's player-controlled entity when possible, otherwise the instigator entity or its hierarchy root. It then compares the origin to the hit position. With default **Allow outgoing**:
- An outside attacker damaging an inside target crosses the sphere: absorb damage, subtract shield HP, and pass only bleed-through / excess after depletion.
- An inside attacker shooting outside passes through without charging the shield.
- Attacks with a line that doesn't cross the sphere proceed normally.
- Shield collapsed, broken, destroyed, or powered off: proceed normally.

An unowned projectile or a custom weapon whose instigator cannot be resolved may bypass this filter. Some custom weapons might not use the vanilla damage manager. These still require explicit integration.

## Test order in Workbench
1. **Back up V3.6** and compile scripts with the new `CGS_ShieldVanillaDamageFilter.c`. If compilation fails, send exact errors.
2. Place a fresh GM generator; set shield HP 5,000; range 75 m; block Kinetic ON; kinetic cost 1; bleed-through 0; and power it up.
3. Place player A outside and player B inside, shoot B with **vanilla bullets**, and observe whether B is protected while GM-visible shield HP decreases. Use a single shot first.
4. Swap shooter and target: player B inside should be able to shoot player A outside without draining shield HP.
5. Change GM Kinetic OFF; incoming bullets should hit through normally.
6. With Kinetic ON, set bleed-through 0.5: a 100-point incoming hit should consume 50 shield HP and deliver 50 damage, if game/weapon uses the base context raw value.
7. Reduce HP and shoot until shield collapses. Verify damage passes after collapse, then regeneration and reactivation work.
8. Repeat with an AI shooter, a seated vehicle shooter, a vanilla rocket, and a second connected player. Confirm late-join HP and bubble state.
9. Turn the generator OFF; confirm startup/shutdown delays and hum still work.

## Caveats
- This code is an **untested Enfusion compile/runtime integration**; it is not verified in Workbench or a dedicated server.
- This hook runs when target damage is processed, not when the projectile visually touches the sphere. Tracers, surface impact VFX, and missiles may still appear to pass through.
- Some explosions generate multiple victim/hitzone damage events. Current V3.7 charges shield HP for each event; it does **not** consolidate one explosion into one shield hit.
- Missing/incorrect instigator entity positions, custom projectile pipelines, and subclass overrides that don't call `super.HijackDamageHandling` can bypass interception.
- Protection is tied to the actual sphere crossing, not an unconditional inside/outside safezone. The shield does not prevent friendly hits inside.
- The older physical rocket/missile scanner remains enabled, with its existing 250 ms detection limitations.
- Changes are isolated to a **single new script file**. Existing V3.6 sound, mesh, shield bubble visual, radius scaling, GM controls, and timers are untouched.
