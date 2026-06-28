# 2026-06-28 022 Expand Room And Add Shell Textures

## Date

2026-06-28

## Goal

Expand the tavern depth, move the bar to the back-center area, lift floor objects above the floor, and add simple procedural textures to the clean wall and door geometry.

## Changed Files

- `source/assets/scenes/scene.json`
- `source/assets/models/clean_wall_panel.obj`
- `source/assets/models/clean_ceiling_panel.obj`
- `source/assets/models/clean_door_panel.obj`
- `source/assets/textures/stone_wall.png`
- `source/assets/textures/wood_door.png`
- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-28-022-expand-room-and-add-shell-textures.md`

## Important Design Decisions

- Kept the entrance at `z = 4.5` and extended the back wall to `z = -11.25`, making the room depth 1.5 times larger than the previous `9` unit depth.
- Moved the floor and ceiling centers to `z = -3.375` and scaled them to cover the larger footprint.
- Set the wall height to `4.88`, ceiling height to `4.95`, and chandelier height to `4.15`.
- Moved the bar counter to the back center at `z = -9.35`.
- Moved the NPC placeholder behind the bar to `z = -10.15`.
- Lifted ground-level furniture and props by `0.16` on the Y axis to reduce visible floor clipping.
- Generated two simple procedural textures:
  - `stone_wall.png` for clean wall and ceiling panels
  - `wood_door.png` for the clean front door panel
- Updated clean OBJ UV coordinates from small atlas samples to full `0..1` UVs so the new textures are visible across each panel.
- Set the debug camera height clamp to `-0.5f` through `5.4f`, giving extra space below the floor and above the ceiling for clipping checks.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the scene now declares:
  - 27 models
  - 3 textures
  - 34 instances
- Verified that all referenced model and texture files exist under `source/assets`.
- Verified that `stone_wall.png` and `wood_door.png` were generated under `source/assets/textures`.
- Statically checked that `cameraPos.y` is now clamped between `-0.5f` and `5.4f`.
- Command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required for visual verification.

## Notes For Exam Review

- The larger room makes space for the final tavern composition: bar counter, bartender/NPC, bottles, shelves, tables, and interaction path.
- The procedural textures are project-owned assets and avoid dependency on external image licensing.
- The clean shell remains simple OBJ geometry, while detailed tavern props continue to use course-provided `.MGCG` assets.
