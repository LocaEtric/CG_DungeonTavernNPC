# 2026-06-28 012 Adjust Debug Camera

## Goal

Move the temporary fixed camera inside the tavern so the newly composed scene is visible before first-person navigation is implemented.

## Changed Files

- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-28-012-adjust-debug-camera.md`

## Important Design Decisions

- Runtime output showed all 23 models, 23 textures, and 33 scene instances were loaded successfully.
- The large gray area was likely caused by the fixed camera at `[0.0, 1.0, 5.0]` looking through the entrance side, where the scene now contains a door and nearby wall geometry.
- Changed only the temporary debug `glm::lookAt` values:
  - camera position: `[0.0, 1.7, 2.6]`
  - target: `[0.0, 0.9, -1.8]`
- This is not the final camera implementation. The next camera milestone should replace this fixed view with first-person movement and yaw/pitch control.

## Tested Or Verified

- Source code location was checked after editing.
- No local C++ build was run. The user should manually build/run in CLion to verify the new view.

## Notes For Exam Review

- This is a temporary scene-debugging view.
- It helps inspect the first tavern layout before implementing the real first-person camera.
