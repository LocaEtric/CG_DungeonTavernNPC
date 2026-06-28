# 2026-06-28 013 Use Shared Texture Atlas

## Goal

Fix the first tavern scene rendering gray by restoring the shared Dungeon texture atlas and using it for the selected `.MGCG` assets.

## Changed Files

- `source/assets/models/Textures.png`
- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-013-use-shared-texture-atlas.md`

## Important Design Decisions

- The selected `.MGCG` models loaded correctly, but rendering appeared gray because the scene used each model's same-name `.png` preview image as the shader texture.
- Those same-name `.png` files are useful previews, but they are not the proper UV atlas for the models.
- Restored the course-provided shared Dungeon atlas from `E:\Downloads\Dungeon\Textures.png` to `source/assets/models/Textures.png`.
- Reduced `scene.json` texture declarations from 23 individual preview textures to one shared texture:
  - `dungeon_palette`
  - `assets/models/Textures.png`
- Updated all scene instances to use `dungeon_palette`.
- Kept the per-model `.png` preview files in `source/assets/models` because the user explicitly requested keeping preview images for selected assets.

## Tested Or Verified

- `source/assets/scenes/scene.json` parses successfully.
- The scene still declares 23 models and 33 instances.
- The scene now declares 1 texture: `assets/models/Textures.png`.
- All 33 instances reference `dungeon_palette`.
- Manually synchronized `source/assets` to `cmake-build-debug/assets` for the current runtime directory.
- Verified build runtime assets contain:
  - 1 scene texture reference
  - `Textures.png`
  - 47 model-directory files total: 46 selected `.mgcg/.png` files plus the shared atlas

## Notes For Exam Review

- The rendering pipeline samples one texture atlas through the fragment shader.
- The model UVs select colors from the shared atlas.
- The same-name `.png` files are previews and should not be described as the actual material textures for these `.MGCG` models.
