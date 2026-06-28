# 2026-06-28 023 Improve Tavern Atmosphere And Bar

## Date

2026-06-28

## Goal

Improve the dungeon tavern atmosphere, reduce floor clipping further, and replace the placeholder box bar with a more readable table-based bar counter.

## Changed Files

- `source/assets/scenes/scene.json`
- `source/assets/textures/stone_wall.png`
- `source/assets/textures/wood_door.png`
- `0.Documentation/DevelopmentLog/2026-06-28-023-improve-tavern-atmosphere-and-bar.md`

## Important Design Decisions

- Lowered the chandelier from `y = 4.15` to `y = 4.05` after visual inspection.
- Regenerated the procedural wall texture with darker, less uniform stone colors and stronger mortar lines to better match a dungeon tavern.
- Regenerated the procedural door texture with darker wood planks and darker metal bands.
- Removed the previous placeholder bar made from three large box instances:
  - `bar_counter_left`
  - `bar_counter_center`
  - `bar_counter_right`
- Added a new bar counter made from:
  - one long `bench_table` instance: `bar_counter_table`
  - four small `wooden_box` support instances under the table
- Raised most floor-level furniture and props by an additional `0.34` on the Y axis. Combined with the previous lift, this puts them about `0.5` units above their earlier placement for easier clipping inspection.
- Repositioned bar-top bottles and jug to the new counter height.
- Kept the NPC placeholder behind the back-center bar.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the scene now declares:
  - 27 models
  - 3 textures
  - 36 instances
- Verified that all scene model and texture ids resolve.
- Visually inspected the regenerated `stone_wall.png` and `wood_door.png`.
- Command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required for final visual verification.

## Notes For Exam Review

- The bar is now built from reusable scene instances instead of a single custom model, showing model reuse with different transforms.
- The darker procedural textures are project-owned and easy to explain.
- The extra Y lift is a scene tuning step; final placement can be lowered later after model origins are understood visually.
