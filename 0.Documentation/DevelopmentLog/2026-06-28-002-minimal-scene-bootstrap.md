# 2026-06-28 002 Minimal Scene Bootstrap

## Goal

Create the first loadable scene file so the Vulkan skeleton can render a minimal textured object before adding tavern layout, camera movement, and NPC interaction.

## Changed Files

- `source/assets/scenes/scene.json`

## Design Decisions

- Added the missing `scene.json` expected by `SC.init(..., "assets/scenes/scene.json")` in `source/src/main.cpp`.
- Used the existing course-provided `floor_Mesh.7127.mgcg` model because `Plane.gltf` references `Plane.bin`, which is not currently present in `source/assets/models`.
- Used `floor_Mesh.7127.png` as the first albedo texture so model and texture come from the same asset pair.
- Kept the scene to a single instance using the existing `VDposUV` vertex descriptor and `BlinnPos` technique to verify the current rendering path before extending the project.

## Verification

- Parsed `source/assets/scenes/scene.json` with PowerShell `ConvertFrom-Json`; the JSON is valid.
- Ran the existing CMake build through the CLion-bundled CMake executable.
- Shader compilation and SPIR-V copy completed successfully.
- After adding the missing JSON dependency and framework implementation unit, the user rebuilt successfully from CLion.
- The application runs and prints the expected framework diagnostics: Vulkan instance/device discovery, shader loading, `scene.json` parsing, MGCG model loading, texture loading, descriptor setup, and text system initialization.

## Exam Review Notes

- This step verifies the basic scene setup path: scene JSON parsing, model loading, texture loading, descriptor binding, command buffer draw calls, and the current simple shader pipeline.
- The scene is intentionally minimal so later problems in camera, lighting, or interaction can be isolated from asset loading.
