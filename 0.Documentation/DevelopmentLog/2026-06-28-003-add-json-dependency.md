# 2026-06-28 003 Add JSON Dependency

## Goal

Fix the build error caused by the missing `json.hpp` header required by the course skeleton's scene loading code.

## Changed Files

- `CMakeLists.txt`

## Design Decisions

- Did not copy `json.hpp` into `source/include` because project rules say not to modify files under that folder.
- Added a CMake step that downloads the official single-header `json.hpp` into the build directory.
- Exposed that build-directory folder as an include directory so the existing skeleton include style `#include <json.hpp>` continues to work without changing framework headers.
- Avoided adding the header to `source/include` because that folder belongs to the provided skeleton/framework area.

## Verification

- CMake configure now completes successfully and adds `cmake-build-debug/_deps/nlohmann_json_single` to the target include path.
- A follow-up build command reaches `main.cpp` with the new include path present.
- The user rebuilt from CLion and passed the previous `json.hpp` missing-header error.

## Exam Review Notes

- `json.hpp` is a third-party header used by `tiny_gltf.h` and the provided `Scene.hpp` loader.
- This change is build configuration only; it does not alter Vulkan rendering behavior.
