# 2026-06-28 031 Rescale MGCG Shell Preview

## Date

2026-06-28

## Goal

Fix the temporary MGCG wall and door preview after the clean OBJ shell transforms made the MGCG wall and door modules render far too large.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-031-rescale-mgcg-shell-preview.md`

## Important Design Decisions

- Kept the room footprint based on the tuned clean OBJ layout:
  - left/right walls around `x = +/-4.5`
  - front wall around `z = 4.5`
  - back wall around `z = -11.25`
- Replaced the oversized clean-panel scale values with MGCG-appropriate scales:
  - back wall: `[1.8, 2.05, 1.2]`
  - side walls: `[3.15, 2.05, 1.2]`
  - front wall sections: `[0.55, 2.05, 1.2]`
  - door: `[1.1, 1.1, 1.1]`
- Added paired outer wall instances so both sides can show stone-brick material:
  - `back_wall_outer`
  - `left_wall_outer`
  - `right_wall_outer`
  - `front_left_wall_outer`
  - `front_right_wall_outer`
- Added `front_door_outer` so the entrance can be seen from the outside as well.
- Did not modify the five fixed wall candle positions.
- Kept `ceiling_clean` as clean OBJ for now.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified wall and door instances now use MGCG models with reduced scales.
- Verified five wall candle positions remained unchanged.
- Ran the `copy_assets` target with the CLion-bundled CMake executable to synchronize `cmake-build-debug/assets`.
- Runtime visual verification is required in CLion to check exact wall coverage, gaps, and possible overlap.

## Notes For Exam Review

- This step shows the difference between scaling a custom unit OBJ panel and scaling a course-provided MGCG module.
- The paired-wall approach is still a scene-composition solution, not a shader or framework change.
- If this preview is visually acceptable, the MGCG shell can be kept; otherwise the clean OBJ shell remains the more controllable final option.
