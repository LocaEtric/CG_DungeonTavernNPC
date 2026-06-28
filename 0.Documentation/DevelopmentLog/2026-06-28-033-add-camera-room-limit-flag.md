# 2026-06-28 033 Add Camera Room Limit Flag

## Date

2026-06-28

## Goal

Add a simple flag in the main application code to switch between a room-limited camera and a free debug camera.

## Changed Files

- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-28-033-add-camera-room-limit-flag.md`

## Important Design Decisions

- Did not modify `source/assets/scenes/scene.json` because the wall, roof, and door positions were manually tuned and fixed by the user.
- Added `cameraRoomLimitFlag`:
  - `1`: clamp the camera inside the room
  - `0`: keep the debug/free camera behavior
- Added room bounds based on the tuned interior shell:
  - minimum: `[-4.55, 0.35, -11.25]`
  - maximum: `[4.55, 4.25, 4.75]`
- The clamp is applied after movement input and before building the `glm::lookAt` view matrix.
- This is a simple boundary clamp, not collision detection against every wall mesh.

## Tested Or Verified

- Searched `source/src/main.cpp` to confirm the new flag and clamp are present.
- Attempted a MinGW `-fsyntax-only` check, but the local compiler exited with code 1 and no diagnostic, matching the previously documented local toolchain issue.
- Manual CLion build/run verification is required to confirm the camera remains inside the tuned room when `cameraRoomLimitFlag = 1`.

## Notes For Exam Review

- This feature is part of camera/view/projection and interaction control logic.
- The camera position is still updated by first-person movement, then constrained by a simple axis-aligned bounding box.
- For debugging wall placement, set `cameraRoomLimitFlag` to `0`.
