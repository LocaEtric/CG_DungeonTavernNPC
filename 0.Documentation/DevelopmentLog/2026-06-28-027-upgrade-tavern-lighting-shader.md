# 2026-06-28 027 Upgrade Tavern Lighting Shader

## Date

2026-06-28

## Goal

Strengthen the main graphics pipeline for exam discussion by replacing the temporary shader names and expanding the single existing pipeline with a clearer tavern lighting model.

## Changed Files

- `source/src/main.cpp`
- `source/shaders/tavern_lit.vert`
- `source/shaders/tavern_lit.frag`
- Removed old source shader names:
  - `source/shaders/toChangeSimplePos.vert`
  - `source/shaders/toChangeBlinnFromPos.frag`

## Important Design Decisions

- Kept only one graphics pipeline for now. A second pipeline can be added later, but this step focuses on making the current pipeline stronger and easier to explain.
- Renamed the project shader files to `tavern_lit.vert` and `tavern_lit.frag` so the source no longer uses skeleton-style `toChange...` names.
- Expanded `GlobalUniformBufferObject` from a single moving directional light to explicit, 16-byte aligned lighting data:
  - camera position
  - ambient color
  - directional light direction and color
  - six point light positions
  - six point light colors
  - material parameters
  - point light count
- Used `vec4`/`ivec4`-style fields in both C++ and GLSL to keep uniform buffer alignment simple to explain during the exam.
- Used the five fixed wall candle positions from the current scene as point lights:
  - `wall_candle_backwall`: `[0.0, 3.35, -11.15]`
  - `wall_candle_FrontLeft`: `[-4.40, 2.15, -7.20]`
  - `wall_candle_FrontRight`: `[4.40, 2.15, -7.20]`
  - `wall_candle_RearLeft`: `[-4.40, 2.15, 1.80]`
  - `wall_candle_RearRight`: `[4.40, 2.15, 1.80]`
- Added the ceiling chandelier as the sixth point light at `[0.0, 3.85, -2.70]`.
- The fragment shader now computes:
  - texture albedo sampling
  - ambient indirect approximation
  - weak cool directional light
  - warm point lights with distance attenuation
  - Blinn specular highlights
  - tone mapping and gamma correction

## Tested Or Verified

- Compiled the new shaders directly with `glslc`:
  - `source/shaders/tavern_lit.vert`
  - `source/shaders/tavern_lit.frag`
- Ran the CLion-bundled CMake build command. CMake reconfigured successfully, compiled both new shaders, copied the new SPIR-V files, and copied assets.
- The full C++ build stopped while compiling `source/src/main.cpp` with MinGW exit code 1 but no diagnostic message. This matches the previously documented local toolchain issue, so manual CLion build/run verification is required.

## Notes For Exam Review

- The main render pipeline now has a clear explanation path:
  - `Vertex` defines position, normal, and UV input attributes.
  - `UniformBufferObject` stores per-instance world and MVP matrices.
  - `GlobalUniformBufferObject` stores scene-level camera, light, and material parameters.
  - `tavern_lit.vert` transforms local mesh coordinates into clip space and passes world position, transformed normal, and UV.
  - `tavern_lit.frag` combines sampled texture color with ambient, directional, and point-light Blinn shading.
- The five wall candles are now not only visible scene props but also real shader light sources.
- This step improves the shader/pipeline part of the project without touching NPC interaction logic.
