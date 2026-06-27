# 2026-06-28 004 Framework Implementation Unit

## Goal

Fix linker errors caused by missing framework implementation symbols.

## Changed Files

- `source/src/Libs.cpp`

## Design Decisions

- Added one source file that defines the framework implementation macros before including the provided framework headers.
- Kept all implementation logic in `source/src` instead of modifying `source/include`.
- Used the established single-translation-unit pattern required by header implementation guards such as `STARTER_IMPLEMENTATION`, `SCENE_IMPLEMENTATION`, and `TEXTMAKER_IMPLEMENTATION`.

## Verification

- Reconfigured CMake so `source/src/Libs.cpp` is included by the existing `file(GLOB SRC_FILES ...)` source list.
- The previous linker errors show that `main.cpp` compiled successfully but the framework implementations were not linked.
- The user rebuilt successfully from CLion after reloading CMake.
- The previous undefined references to `BaseProject`, `Scene`, `TextMaker`, `DescriptorSet`, `Pipeline`, and `RenderPass` were resolved.

## Exam Review Notes

- This file does not add project behavior; it only makes the provided skeleton framework link correctly.
- Application-specific logic remains in `main.cpp` and future project files.
