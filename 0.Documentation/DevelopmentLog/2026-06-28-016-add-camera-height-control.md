# 2026-06-28 016 Add Camera Height Control

## Date

2026-06-28

## Goal

Add vertical camera movement so the scene can be inspected from slightly different heights during layout tuning.

## Changed Files

- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-28-016-add-camera-height-control.md`

## Important Design Decisions

- Reused the skeleton `getSixAxis()` vertical movement mapping:
  - `R` moves the camera upward.
  - `F` moves the camera downward.
- Added a separate `verticalSpeed` value so height adjustment can be tuned independently from horizontal walking.
- Clamped only the camera height between `0.8` and `2.4`.
- Did not add X/Z room boundary clamping yet because the scene layout is still being tuned.

## Tested Or Verified

- Statically checked that horizontal movement and yaw/pitch camera rotation are unchanged.
- Statically checked that height adjustment is applied before building the `glm::lookAt` view matrix.
- Local command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required.

## Notes For Exam Review

- The control uses an existing skeleton input axis instead of modifying the framework.
- This is a simple debug-friendly first-person camera height control.
- The height clamp keeps the camera within a reasonable player-view range.
