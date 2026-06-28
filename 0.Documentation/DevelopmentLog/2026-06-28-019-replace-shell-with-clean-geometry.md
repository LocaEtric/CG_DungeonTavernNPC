# 2026-06-28 019 Replace Shell With Clean Geometry

## Date

2026-06-28

## Goal

Remove the visible gray and red backplates from the room shell by replacing problematic wall, ceiling, and door assets with simple project-owned geometry.

## Changed Files

- `source/assets/models/clean_wall_panel.obj`
- `source/assets/models/clean_ceiling_panel.obj`
- `source/assets/models/clean_door_panel.obj`
- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-019-replace-shell-with-clean-geometry.md`

## Important Design Decisions

- The gray wall backs and red door backplate are part of the original `.MGCG` asset geometry, not separate scene instances. They cannot be removed cleanly from `scene.json` alone.
- Added three simple OBJ meshes owned by this project:
  - `clean_wall_panel.obj`
  - `clean_ceiling_panel.obj`
  - `clean_door_panel.obj`
- Each OBJ includes positions, UVs, and normals so it works with the existing `VDposNormUV` vertex descriptor and Blinn-Phong shader.
- Replaced the previous MGCG wall, corner, ceiling, and door instances with clean rectangular panels:
  - back wall
  - left wall
  - right wall
  - front left wall
  - front right wall
  - ceiling
  - front door
- Enlarged the shell to a simple `9 x 9` room footprint and raised the ceiling to `y = 3.78`.
- Raised the chandelier to `y = 3.18` so it hangs under the higher ceiling.
- Kept the original MGCG furniture, props, candles, floor, and tavern objects unchanged.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the scene now declares:
  - 27 models
  - 1 shared texture
  - 34 instances
- Verified that all instance model and texture references resolve to declared scene entries.
- Verified that all referenced model and texture files exist under `source/assets`.
- Checked that the new OBJ files include vertices, UV coordinates, normals, and triangle faces.
- Command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required to verify the exact visual result.

## Notes For Exam Review

- This change keeps the room shell simple and explainable: it is regular OBJ geometry loaded through the same scene/model system as other meshes.
- The custom OBJ shell avoids modifying course framework files and avoids editing the encoded `.MGCG` assets.
- The shell uses the existing shared texture atlas coordinates, so it remains compatible with the current single-texture descriptor setup.
