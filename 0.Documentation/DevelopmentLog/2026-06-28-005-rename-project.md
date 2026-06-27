# 2026-06-28 005 Rename Project

## Goal

Replace the remaining skeleton placeholder names with project-specific names before continuing with rendering, camera, and interaction features.

## Changed Files

- `CMakeLists.txt`
- `source/src/main.cpp`

## Design Decisions

- Renamed the CMake project from `SkeletonToChange` to `DungeonTavernNPC`.
- Renamed the main application class from `Skeleton26ReplaceName` to `DungeonTavernNPCApp`.
- Kept the existing window title because it already identifies the project.
- Did not change rendering logic, scene loading, shaders, assets, or framework files.

## Verification

- Confirmed `cmake-build-debug/CMakeCache.txt` now contains `CMAKE_PROJECT_NAME=DungeonTavernNPC`.
- Ran the CLion-bundled CMake command:
  `D:/Program Files (x86)/JetBrains/CLion 2026.1.3/bin/cmake/win/x64/bin/cmake.exe --build cmake-build-debug`
- The build uses the renamed target directory `CMakeFiles/DungeonTavernNPC.dir`, so the project rename is active.
- Build verification did not complete because MinGW `g++.exe` exited with code 1 while compiling both `Libs.cpp` and `main.cpp`, but did not print a C++ diagnostic message.
- The user then rebuilt and ran the project successfully from CLion using `cmake-build-debug/DungeonTavernNPC.exe`.
- Runtime output confirms shader loading, `scene.json` parsing, one MGCG floor model loaded, one texture loaded, one scene instance created, and text system initialization completed.
- The application window displays the renamed title and the static floor scene with the FPS text.
- No further runtime feature was changed in this step.

## Exam Review Notes

- This step removes skeleton placeholder naming while preserving the course framework structure.
- The application-specific entry point is now easier to identify during code review and exam explanation.
