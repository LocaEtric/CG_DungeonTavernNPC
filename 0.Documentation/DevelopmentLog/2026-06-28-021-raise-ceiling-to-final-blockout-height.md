# 2026-06-28 021 Raise Ceiling To Final Blockout Height

## Date

2026-06-28

## Goal

Raise the tavern ceiling and chandelier again, and align the R/F camera height limit with the ceiling height for inspection.

## Changed Files

- `source/assets/scenes/scene.json`
- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-28-021-raise-ceiling-to-final-blockout-height.md`

## Important Design Decisions

- Increased the clean wall panel Y scale from `4.15` to `4.75`.
- Raised the clean ceiling from `y = 4.22` to `y = 4.82`.
- Raised the chandelier from `y = 3.62` to `y = 4.12`.
- Updated the vertical camera clamp upper limit from `3.25f` to `4.82f`, matching the ceiling height used in the scene blockout.
- Kept the current clean OBJ shell because it avoids the unwanted `.MGCG` wall and door backplates.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the scene still declares:
  - 27 models
  - 1 shared texture
  - 34 instances
- Verified that scene model and texture ids still resolve.
- Statically checked that `cameraPos.y` is clamped between `0.8f` and `4.82f`.
- Command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required for visual verification.

## Notes For Exam Review

- The ceiling and camera limit are intentionally synchronized for easier debugging and presentation inspection.
- The final first-person walking camera can later use a lower gameplay height while keeping the debug height mode if needed.
