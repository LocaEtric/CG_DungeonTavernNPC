# 2026-06-28 010 Compose First Tavern Scene

## Goal

Replace the one-floor test scene with a first readable tavern layout using only the pruned asset set.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-010-compose-first-tavern-scene.md`

## Important Design Decisions

- Registered all 23 selected models and matching textures in `scene.json`.
- Kept a single `BlinnPhong` technique for every asset so the scene remains simple to explain and uses the existing position/normal/UV vertex format.
- Built a compact tavern layout with:
  - a scaled main floor
  - three wall sections and two rear corners
  - a front door
  - a box-based bar counter
  - a temporary NPC marker behind the bar
  - cupboard, bottles, jugs, tables, chairs, barrels, boxes, candles, wall candles, and a chandelier
- Placed `npc_placeholder` at world position `[0.0, 0.95, -3.05]`. This is the intended first interaction target for the later NPC distance check.
- Prioritized clear first-pass composition over final art polish. Model scales and rotations may need visual tuning after manual run.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Checked that all referenced model and texture files exist under `source/assets/models`.
- Checked that all instance `model` and `texture` ids resolve to declared entries.
- Did not run a local build because this step changes scene data only and the project has a known user-side CLion build/run path. Manual build/run should be used to verify final visual placement.

## Notes For Exam Review

- This scene demonstrates mesh/model loading, texture mapping, scene setup, and per-instance transforms through `scene.json`.
- The repeated objects are intentional: they show that the same mesh and texture can be reused with different world transforms.
- The `npc_placeholder` is not final character art. It is a stable world-space marker for the next milestone: first-person camera movement and distance-based interaction.
