# 2026-06-28 006 Improve Vertex Format

## Goal

Replace the temporary position-and-UV vertex format with a format that also carries mesh normals, so the lighting shader can use real model normals instead of deriving normals from screen-space derivatives.

## Changed Files

- `source/src/main.cpp`
- `source/shaders/toChangeSimplePos.vert`
- `source/shaders/toChangeBlinnFromPos.frag`
- `source/assets/scenes/scene.json`

## Design Decisions

- Added `glm::vec3 norm` to the project vertex structure.
- Updated the Vulkan vertex descriptor to expose three attributes:
  - location 0: position
  - location 1: normal
  - location 2: UV
- Renamed the scene vertex descriptor reference from `VDposUV` to `VDposNormUV` so the scene file describes the new layout clearly.
- Renamed the scene technique reference from `BlinnPos` to `BlinnPhong` because the shader now uses explicit normals for Blinn-style lighting.
- Updated the vertex shader to transform normals into world space using the inverse-transpose of the model matrix.
- Updated the fragment shader to normalize the interpolated world-space normal and removed the previous `dFdx` / `dFdy` normal approximation.
- Kept the existing texture sampling, ambient term, diffuse term, specular term, descriptor sets, and render pass structure unchanged.

## Verification

- `source/assets/scenes/scene.json` was parsed successfully with PowerShell `ConvertFrom-Json`.
- The CLion-bundled CMake build command was run:
  `D:/Program Files (x86)/JetBrains/CLion 2026.1.3/bin/cmake/win/x64/bin/cmake.exe --build cmake-build-debug`
- Shader compilation completed successfully:
  - `toChangeSimplePos.vert`
  - `toChangeBlinnFromPos.frag`
- The updated SPIR-V files were copied into `cmake-build-debug/shaders/`.
- Full C++ build verification did not complete because the CLion-bundled MinGW `g++.exe` returned exit code 1 while compiling `main.cpp` without printing any C++ diagnostic.
- A minimal standalone hello-world syntax check with the same `g++.exe` also returned exit code 1 without diagnostics, so the current blocker appears to be the local compiler/toolchain state rather than a reported source-level error in this change.

## Exam Review Notes

- The project now has an explicit vertex format section that can be explained as position, normal, and UV.
- Normals are loaded through the existing course `Scene` / `Model` framework because `Starter.hpp` already supports the `NORMAL` vertex usage and GLTF/MGCG `"NORMAL"` attribute.
- The vertex shader is responsible for coordinate transforms:
  - position is transformed by the MVP matrix for rasterization
  - position is also transformed by the model matrix for world-space lighting
  - normal is transformed by the inverse-transpose model matrix
- The fragment shader lighting is now easier to explain during the exam because it uses the mesh normal directly for Lambert diffuse and Blinn-style specular terms.
