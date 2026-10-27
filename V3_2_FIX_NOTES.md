# CGS Shield Generator V3.2 – visual, range and editable-prefab fix

Applied to the V3.1 GM registry package; project GUID and editor registry remain unchanged.

1. The *player-provided* `PrefabsEditable/Auto/Shields/E_CGS_ShieldGenerator.et` had an extra MeshObject pointing at `Common/Models/Default.xob`. This extra override is removed, leaving the preview component and the inherited generator model. The BASE generator prefab `Prefabs/Shields/CGS_ShieldGenerator.et` is intentionally NOT edited; any custom base model or collision you have set up is preserved by using the patch-only ZIP.
2. The bubble material `Assets/Shield/Data/CGS_Shield_Bubble.emat` used `AlphaMul 0.002` (virtually invisible). Raised to `0.18` for visual confirmation during tests.
3. `CGS_ShieldGeneratorComponent.c`: uses replicated shield radius for bubble scale, falls back to prefab radius before initial replication, refreshes the local bubble immediately and after changes, and avoids client-side temporary zero shield HP hiding the bubble after the power countdown. Added bounds-sanity fallback similar to Mech Core.
4. Added event-based diagnostic output for actual radius changes, startup-complete, successful bubble spawn, and failed resource loads. No per-frame print spam.

## Recommended install

For a project with your own custom base mesh, use **CGS_Shield_Gen_V3_2_Patch_Only.zip** rather than replacing the whole mod. It replaces only three files (above). Shut down Workbench, copy those 3 paths into your existing `CGS Shield Gen` project, reopen, and let it recompile.

## Test in Game Master

- Spawn fresh `CGS Shield Generator` through Systems, no old saved generator instances.
- Start at radius 75m. Increase to 100m using the physical radius action or the GM radius slider.
- Activate: during the first 15–30 seconds no bubble is expected; at 0 the blue dome should become visible, with the selected radius. Shutdown reverses the visible state after its configured delay.
- Check console for `[CGS SHIELD GEN] Transition complete`, `[CGS SHIELD GEN] Radius set`, and `[CGS SHIELD GEN] Bubble spawned`; if there is no bubble at all, check for `FAILED to load` / `FAILED to spawn` / `Unusable bubble mesh bounds` warnings. A dedicated server has no graphics; the bubble-spawn message will come from a client, not a headless server.
- If the dome is the wrong physical size, compare `model radius` and `scale` in the bubble-spawn line; set `m_fBubbleMeshRadius` explicitly on the base prefab if needed.

## Unfinished

Vanilla/modded projectile interception is not yet wired. This patch addresses the visible bubble and control path, not the actual physics/damage shield. This package has been syntax/static-checked and zip-tested, but not compiled in Enfusion Workbench or tested in multiplayer.
