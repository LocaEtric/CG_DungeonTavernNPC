# 2026-06-28 030 Preview MGCG Wall Door Materials

## Date

2026-06-28

## Goal

Temporarily switch the wall and door shell instances back to course-provided MGCG models so the original asset UVs and dungeon palette material can be visually compared against the clean OBJ shell.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-030-preview-mgcg-wall-door-materials.md`

## Important Design Decisions

- Changed the five wall shell instances from `clean_wall_panel` to `stone_wall`.
- Changed the front door shell instance from `clean_door_panel` to `door`.
- Kept all of those instances using `dungeon_palette`.
- Left `ceiling_clean` as `clean_ceiling_panel` for now so this step focuses only on wall and door material comparison.
- Did not modify the fixed five wall candle positions.
- This is a temporary visual comparison step. Earlier logs noted that the MGCG wall and door meshes may bring back unwanted backplate geometry.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified the changed shell instances:
  - `back_wall_clean`: `stone_wall`
  - `left_wall_clean`: `stone_wall`
  - `right_wall_clean`: `stone_wall`
  - `front_left_wall_clean`: `stone_wall`
  - `front_right_wall_clean`: `stone_wall`
  - `front_door_clean`: `door`
- Verified the five wall candle positions were not changed.
- Ran the `copy_assets` build target with the CLion-bundled CMake executable so the runtime asset directory is synchronized.

## Notes For Exam Review

- This step compares course asset UVs/materials against project-owned clean geometry.
- If the original MGCG material reads better but the geometry causes backplate artifacts, the likely final solution is to keep clean geometry and tune its UVs/material colors rather than using the original MGCG shell directly.
