# 2026-06-28 014 Add Camera Pitch

## Date

2026-06-28

## Goal

Extend the first-person camera from yaw-only rotation to yaw and pitch rotation, so the player can look left/right and up/down.

## Changed Files

- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-28-014-add-camera-pitch.md`

## Important Design Decisions

- Added a `pitch` camera state next to the existing `yaw` state.
- Updated `pitch` from the vertical rotation axis returned by the skeleton `getSixAxis()` helper.
- Clamped `pitch` between `-75` and `75` degrees to avoid flipping the camera at near-vertical angles.
- Split camera direction into two vectors:
  - `viewForward` includes yaw and pitch and is used for `glm::lookAt`.
  - `moveForward` uses yaw only and keeps WASD movement on the tavern floor.
- Did not add room boundary clamping yet because the tavern layout still needs visual tuning.

## Tested Or Verified

- Statically checked that the view matrix is still computed as `glm::lookAt(cameraPos, cameraPos + viewForward, up)`.
- Statically checked that `ViewPrj = Prj * View` is unchanged.
- Statically checked that movement still uses the user's corrected W/S direction.
- Local command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is still required.

## Notes For Exam Review

- The camera is a simple first-person camera with persistent position, yaw, and pitch state.
- The projection matrix is recomputed every frame from FOV, aspect ratio, near plane, and far plane.
- The pitch clamp prevents unstable camera flips while preserving normal first-person look controls.
- Keeping movement horizontal makes navigation easier to explain and avoids accidental flying when looking up or down.
