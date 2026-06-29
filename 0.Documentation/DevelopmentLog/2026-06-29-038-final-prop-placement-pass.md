# 2026-06-29 - Final Prop Placement Pass

## Date

2026-06-29

## Goal

Replace unused floating tabletop props with the final model assets selected for the Dungeon Tavern NPC scene, and add more readable Halloween/dungeon atmosphere objects without moving the already fixed furniture and NPC base positions.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/ProjectPlan.md`
- `0.Documentation/DevelopmentLog/2026-06-29-038-final-prop-placement-pass.md`

## Important Design Decisions

- Kept the existing bar table, right bench table, greeter, bartender, and chandelier transforms as placement anchors.
- Replaced the three old bar-top objects with four drink items:
  - `water_on_bar` using `tipple.005_Mesh.177.mgcg`
  - `wine_bottle_on_bar` using `tipple.010_Mesh.189.mgcg`
  - `wine_glass_on_bar` using `cup.002_Mesh.194.mgcg`
  - `beer_cup_on_bar` using `cup.001_Mesh.193.mgcg`
- Replaced the old right-table book/bottle/candle group with:
  - `potion_on_right_table` using `prop.030_Mesh.7394.mgcg`
  - `globe_on_right_table` using `prop.042_Mesh.7896.mgcg`
- Removed the storage chest and wooden box between the right table and bar area, replacing them with:
  - `right_storage_cage` using `torture.010_Mesh.7090.mgcg`
  - `right_storage_torture_chair` using `torture.004_Mesh.5240.mgcg`
- Added atmosphere props:
  - `crow_on_greeter_arm` using `crow_Mesh.5912.mgcg`
  - `hanging_skeleton_left_wall` using `bones.033_Mesh.700.mgcg`
  - `pumpkin_near_entrance` using `pumpkin.008_Mesh.483.mgcg`
  - `pumpkin_near_right_table` using `pumpkin.010_Mesh.4592.mgcg`
- Corrected the greeter crow placement after the initial push:
  - moved it closer to the greeter arm
  - slightly increased the scale for readability
  - removed the unused `bat_under_chandelier` scene instance
- Chose conservative scales and table heights from existing nearby props:
  - bar-top items use `y = 1.42`
  - right-table items use `y = 1.02`
  - floor decorations use `y = 0.42` to `0.50`
  - hanging props use positions near the existing chandelier and wall-candle height range

## How The Change Was Tested Or Verified

- Parsed `source/assets/scenes/scene.json` with PowerShell `ConvertFrom-Json`; the file parses successfully.
- Checked every instance `model` reference against the registered `models` list; all instance model references are registered.
- Searched the scene file to confirm the old unused instance ids were removed:
  - `bottle_rack_on_bar`
  - `green_bottle_on_bar`
  - `wide_jug_on_bar`
  - `book_on_right_table`
  - `bottle_on_right_table`
  - `table_candle_right`
  - `right_storage_chest`
  - `right_storage_box`
- Local build/run verification was not completed because `cmake` and `ninja` are not available in the current PowerShell PATH, and the attempted CLion CMake lookup did not find an executable quickly enough.

## Notes For Exam Review

- This step is only scene composition. It does not change the Vulkan framework, shader code, descriptor layouts, camera logic, or NPC interaction logic.
- The new props are loaded through the same `scene.json` model registration and instance transform path as the rest of the tavern, so the mesh loading and draw-call explanation remains consistent.
- If a tabletop item still appears too high or low during CLion runtime inspection, adjust only that instance's `translate.y` value in `scene.json`; the furniture anchor positions should stay fixed.
