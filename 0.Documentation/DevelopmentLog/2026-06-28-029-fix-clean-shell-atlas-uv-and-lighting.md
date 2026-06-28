# 2026-06-28 029 Fix Clean Shell Atlas UV And Lighting

## Date

2026-06-28

## Goal

Make the clean OBJ shell sample small, stable regions of the course dungeon palette instead of stretching the full atlas across the walls, and reduce the over-bright foggy lighting effect.

## Changed Files

- `source/assets/models/clean_wall_panel.obj`
- `source/assets/models/clean_ceiling_panel.obj`
- `source/assets/models/clean_door_panel.obj`
- `source/src/main.cpp`
- `source/shaders/tavern_lit.frag`
- `0.Documentation/DevelopmentLog/2026-06-28-029-fix-clean-shell-atlas-uv-and-lighting.md`

## Important Design Decisions

- Kept the shell instances using `dungeon_palette`.
- Changed only the clean OBJ texture coordinates so each shell mesh samples a small area of `assets/models/Textures.png`:
  - wall panel: muted gray-brown atlas area
  - ceiling panel: nearby darker neutral atlas area
  - door panel: brown atlas area
- This avoids showing the whole palette as colored wall stripes while still using the course asset atlas.
- Reduced the global light intensity to avoid a washed-out, translucent-looking scene:
  - lowered ambient color
  - lowered directional light color
  - lowered wall candle and chandelier point light intensities
  - lowered global specular strength
  - reduced shininess
  - increased point-light attenuation
- Kept the five fixed wall candle positions unchanged.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified the clean OBJ files now use atlas subregion UVs rather than full `0..1` UVs.
- Compiled `source/shaders/tavern_lit.vert` and `source/shaders/tavern_lit.frag` directly with `glslc`.
- Runtime visual verification is still required in CLion to judge whether the chosen atlas subregions and lower light intensities look correct.

## Notes For Exam Review

- The issue was not missing vertex colors. The clean OBJ panels had valid UVs, but those UVs covered the entire atlas.
- The fix demonstrates texture mapping control: the same atlas can produce very different results depending on mesh UV coordinates.
- The lighting change keeps the shader model simple: ambient approximation, directional light, point lights with attenuation, and Blinn specular.
