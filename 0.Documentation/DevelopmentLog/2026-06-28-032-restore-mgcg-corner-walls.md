# 2026-06-28 032 Restore MGCG Corner Walls

## Date

2026-06-28

## Goal

Restore corner wall modules after switching the shell preview from clean OBJ panels back to MGCG wall assets.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-032-restore-mgcg-corner-walls.md`

## Important Design Decisions

- Added four corner instances aligned to the current room footprint:
  - `back_left_corner`
  - `back_right_corner`
  - `front_left_corner`
  - `front_right_corner`
- Used the existing course-provided corner models:
  - `corner_wall`
  - `brick_corner`
- Matched the corner wall height to the current MGCG wall preview scale with Y scale `2.05`.
- Kept the fixed five wall candle positions unchanged.
- Kept the current paired straight-wall setup so both inside and outside can show stone-brick material.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified the four corner instances exist with MGCG models and `dungeon_palette`.
- Ran the `copy_assets` target with the CLion-bundled CMake executable to synchronize runtime assets.
- Runtime visual verification is required in CLion to confirm rotations and exact corner coverage.

## Notes For Exam Review

- This is scene composition using existing course assets and per-instance transforms.
- The corner wall models help hide seams between straight wall modules and make the tavern shell read as a closed room.
