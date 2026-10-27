# CGS Shield Gen — initial independent mod package

Project identity is unchanged (`CGSShieldGen`, GUID `6AAA06DA7555B7BD`).

## Included

- `Prefabs/Shields/CGS_ShieldGenerator.et` — destructible GM-editable generator prefab.
- `Prefabs/Shields/CGS_ShieldBubble.et` — local-only blue translucent sphere proxy.
- `Scripts/Game/CGS_ShieldGen/CGS_ShieldGeneratorComponent.c` — generator replicated power, 25–150 m radius, shield capacity, regeneration, break and visual.
- `Scripts/Game/CGS_ShieldGen/CGS_ShieldGeneratorAction.c` — physical start/stop interaction.
- `Scripts/Game/CGS_ShieldGen/CGS_ShieldGeneratorPlayerNetwork.c` — dedicated-server RPC with physical-distance check.
- `Scripts/Game/CGS_ShieldGen/CGS_ShieldGeneratorGMAttributes.c` — power/radius/capacity/regen GM controls.
- `Scripts/Game/CGS_ShieldGen/CGS_ShieldGeneratorDeployableComponent.c` — player inventory deploy hook to wire to an inventory-item prefab.
- `Configs/Editor/...` & `Prefabs/Editor/Modes/EditorModeEdit.et` — examples from Auto Turrets and BFA Drops for GM attribute and spawn registration.
- `Assets/Generator` — **temporary BFA Drops console mesh** (copied from user-provided reference, with new resource GUIDs); replace with actual shield generator model later.
- `Assets/Shield` — sphere mesh and transparent material adapted from user-provided Mech Core reference, also with new GUIDs.

## Critical status and validation

This is a **Workbench integration/test starter, V2**, not a compiled or validated release. Arma Reforger Enfusion Workbench is not available in this environment.

This first package does NOT intercept vanilla or modded bullets, guided missiles, explosives or beams: the `ServerAbsorb` hook provides state for that future engine/projectile integration but is not connected to weapons. The visible sphere is not a collision or physics shield.

The generator is configured as an editable destructible prefab, but destructive collision/hit-zone behavior must be verified in Workbench after import. The player deployment component is provided but the actual inventory-item prefab and hotbar trigger are not wired yet.

## First test sequence

1. Back up your empty project, then extract this archive into your Arma Reforger addons workspace. The original project identity is preserved. Let Workbench index the new assets.
2. Compile Game scripts. Report exact filename, line number and engine error for any failures.
3. Open `Prefabs/Shields/CGS_ShieldGenerator.et`, check its resource/material references and required components.
4. Spawn the generator in a test world, confirm actions START/STOP toggle a blue translucent bubble. Try 25 m / 75 m / 150 m in the GM edit panel.
5. Test in local host and dedicated server with two clients; check late join, GM remote editing, destruction.
6. After this is stable, connect interceptors for vanilla and modded projectiles and wire a player inventory-item prefab.

## Defaults

- Starts powered **off**
- Radius **75 m** (GM adjustable **25–150 m**)
- Shield capacity **5,000** points; regenerate **100 / s**
- Recharge waits **5 s** after damage; after break waits **15 s**; reactivate at **25%**
- Generator destructible base health **1,500**, separate from shield energy

## Notes

- At 150 m the bubble is 300 m in diameter. It will likely need LOD/transparency optimization on servers.
- Do not give the visual bubble an RplComponent or RigidBody; each client spawns its own local-only visualization.
- Mod dependencies remain vanilla only. Shared source assets in this package have fresh GUIDs to avoid collisions when running BFA Drops and Mech Core together.
- `Configs/Editor/AttributeLists/Edit.conf` and `Prefabs/Editor/Modes/EditorModeEdit.et` may require merging with other mods overriding those same vanilla paths; verify GM integration with your actual mod stack.

## V2: GM editable protection rules and one-way geometry

The generator now has a standalone shield classification (`CGS_ShieldDamageKind.c`) adapted from the user's original Mech Core protection categories. No Mech Core classes or dependencies are required.

Editor attributes added:
- Allow outgoing fire (default ON). Inside-to-outside trajectories ignore the shield.
- Block kinetic / explosive / energy / EMP / other incoming damage individually (all ON by default).
- Damage bleed-through (0% default), and per-type health-cost multipliers: kinetic=1, explosive=1, energy=1, EMP=3, other=1.

`CGS_ShieldGeneratorComponent.IntersectProtectedSegment(start, end, damageKind, out surface)` checks server/client ray crossing against the hemispherical dome: outside-to-inside intersects; inside-to-outside passes by default. `CGS_ShieldInterception.ServerFilterShot()` finds the nearest compatible dome across all generators and applies the shield's replicated health drain.

### Important limits

**The API is not yet connected to the vanilla or modded gun damage pipeline.** This version does **not** automatically block bullets, missiles, or blasts; the shield is not fully functional protective cover yet. A proper vanilla ballistics/hitscan interception hook and blast-specific logic still need to be researched and implemented and then tested against actual game weapons on a dedicated server. The existing visual sphere intentionally has **no physical collider**; otherwise it would block friendly outgoing shots as well.

If integrated by a custom weapon, a returned `true` from `ServerFilterShot` means a surface crossing was found; `damageToTarget` is the damage allowed past the shield. It is **not** an instruction to always cancel the shot: when health is insufficient or bleed-through > 0, a portion may still reach the target. The weapon integration must handle the resulting damage, projectile visuals, and hit effects, not merely call the API.

Game Master attributes rely on editor native WriteVariable authorization; the setters are server-only and not exposed as RPC methods. The editor attribute list / mode overrides must be verified for interoperability in the full mission mod stack.

### V2 tests in Workbench

1. Verify Game script compilation, especially the newly added standalone `CGS_ShieldDamageKind.c` and GM editor classes.
2. Spawn multiple generators as GM, check toggles for Block kinetic / other damage, BLEED, costs and one-way fire. Verify settings replicate after reconnect.
3. Unit test `IntersectProtectedSegment` using ray points above the base: outside->inside returns positive distance, inside->outside returns -1 (default), ray missing sphere returns -1, and rays below the ground plane return -1.
4. Confirm no engine projectile blocking is assumed until actual vanilla and modded interception hooks are wired and tested.

## V2.1 compile fix — 2026-10-21

- Fixed Enfusion unsupported compound operator on `angles[0]` in `CGS_ShieldGeneratorDeployableComponent.c` by using explicit assignment.
- This addresses only the reported `Operator '+=' not supported with array accessor []` error. Other Workbench errors remain unverified.


## V3 — timed startup/shutdown, full GM attributes, player radius controls

- When started, generator enters STARTING for the configured 15–30 seconds (20 s default): no shield before transition finishes.
- When shut down, generator enters STOPPING for the configured 15–30 seconds (20 s default): the shield remains active until the timer ends.
- Switching commands during transition are ignored; state and seconds remaining are replicated from server.
- GM attributes (26 rows): power, radius, capacity, regeneration on/off and amount, regeneration delay, collapse recharge delay, reactivation fraction, startup/shutdown times, player power/radius permissions, outgoing shots, six damage blocking categories, bleed-through, and six per-type damage-cost multipliers.
- GM placeable prefab `PrefabsEditable/Auto/Shields/E_CGS_ShieldGenerator.et` is registered in `Configs/Editor/PlaceableEntities/Systems/Systems.conf`, matching the working Glow Lights pattern.
- The base generator prefab uses the same stock editable-component template / flags pattern as Glow Lights, so generators created elsewhere can also be GM-edited if registered.
- Players near the generator have START / SHUT DOWN, SHIELD RADIUS +5m and SHIELD RADIUS -5m. Server checks interaction distance (4 m), permission, radius bounds, and rate limits requests.
- Operator error on yaw array accessor from V2.1 remains fixed.
- For an existing local project, **delete** legacy `Prefabs/Editor/Modes/EditorModeEdit.et` (and `.meta`) and `Configs/EntityCatalog/PlaceableEntities/CGS_ShieldGenerator.conf` (and `.meta`) from earlier versions before copying this new package to avoid duplicate/incorrect GM registration.

**Not claimed validated:** Workbench compilation and actual GM runtime integration are still untested. The projectile blocking hook is STILL NOT CONNECTED to vanilla bullets or other mods. The deployable inventory item still needs its own inventory prefab/action wiring. Generator structural health is still prefab-controlled (1,500), not a GM slider.

- Added independent TRUE / armor-piercing category and HP cost control (requires a calling weapon interceptor; not a vanilla automatic interception).


## V3.1 — editor-mode registry activation (Mech Bay fix)

**Problem:** V3 included `Configs/Editor/PlaceableEntities/Systems/Systems.conf` and `PrefabsEditable/Auto/Shields/E_CGS_ShieldGenerator.et` but left out `Prefabs/Editor/Modes/EditorModeEdit.et`. In mod stacks where the active EditorModeEdit does not load the new registry, the generator will be missing from GM placement.

**Fix:** Added `Prefabs/Editor/Modes/EditorModeEdit.et` and `.meta` with the stock `EditorModeEdit.et` resource GUID `{59EF8ECAE1DCD417}`. The placing editor component now explicitly references `Configs/Editor/PlaceableEntities/Systems/Systems.conf` resource GUID `{B129EA7AACD442EF}`; that registry includes the editable generator prefab `{71B68A4C75954F9C}PrefabsEditable/Auto/Shields/E_CGS_ShieldGenerator.et`. This follows the same underlying cause/fix that made the user's Mech Bay appear.

**Install:** Copy the whole updated project into the addon (preserve directory structure), replacing the older files. If a legacy `EditorModeEdit.et.meta` used GUID `{56BB9096DA0C45F7}`, replace it with the new `.meta` so Workbench references the vanilla editor-mode GUID.

**Important mod-stack note:** `EditorModeEdit.et` is a shared vanilla override path. Multiple mods changing that resource can conflict by load order, including other custom GM registries (e.g. Mech Bay, BFA Drops). If entries from one addon vanish when all are loaded, merge each mod's registry reference into the effective active editor mode instead of replacing others. This package cannot automatically patch the user's full mod stack.

**Test:** Verify `CGS Shield Generator` appears under GM Systems, is placeable, double-click opens custom controls, changing 25–150 m radius / 15–30 s timers / damage rules replicates on a dedicated server, and player interactions remain visible. This package has not been Workbench-compiled or tested live. The vanilla projectile/damage interception hook is still not connected; registration does not make the dome stop bullets.
