# 2026-06-28 015 Fix Keyboard Pitch Direction

## Date

2026-06-28

## Goal

Correct the keyboard pitch direction so the up arrow looks upward and the down arrow looks downward.

## Changed Files

- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-28-015-fix-keyboard-pitch-direction.md`

## Important Design Decisions

- Kept the yaw, movement, projection, and view-projection logic unchanged.
- Added a small `pitchInput` variable before updating the camera pitch.
- Overrode the skeleton `getSixAxis()` keyboard Up/Down mapping only for camera pitch, because its sign convention was opposite to the intended first-person camera behavior.
- Left mouse pitch input on the existing skeleton mapping.

## Tested Or Verified

- Statically checked that only the pitch input sign handling changed.
- Statically checked that `pitch` is still clamped between `-75` and `75` degrees.
- Local command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required.

## Notes For Exam Review

- The camera keeps a simple first-person control scheme.
- The keyboard pitch correction is local to the project code and does not modify the provided skeleton.
