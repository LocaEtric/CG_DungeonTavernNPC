# 2026-06-28 020 Raise Ceiling And Camera Limit

## Date

2026-06-28

## Goal

Raise the tavern ceiling and chandelier after visual inspection showed the chandelier hanging too low.

## Changed Files

- `source/assets/scenes/scene.json`
- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-28-020-raise-ceiling-and-camera-limit.md`

## Important Design Decisions

- Increased the clean wall panel Y scale from `3.7` to `4.15`.
- Raised the clean ceiling from `y = 3.78` to `y = 4.22`.
- Raised the chandelier from `y = 3.18` to `y = 3.62`.
- Increased the R/F camera height clamp upper limit from `2.4` to `3.25` so the taller room can still be inspected during layout tuning.
- Kept the floor and furniture placement unchanged.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the scene still declares:
  - 27 models
  - 1 shared texture
  - 34 instances
- Verified that scene model and texture ids still resolve.
- Statically checked that the camera height clamp now uses `0.8f` to `3.25f`.
- Command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required for final visual verification.

## Notes For Exam Review

- This is a layout tuning step after replacing the room shell with simple OBJ geometry.
- The R/F height clamp is only for camera inspection and does not require framework changes.
