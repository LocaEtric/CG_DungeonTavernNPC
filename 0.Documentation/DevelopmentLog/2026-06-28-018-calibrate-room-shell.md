# 2026-06-28 018 Calibrate Room Shell

## Date

2026-06-28

## Goal

Fix the visible gaps and scale mismatches in the closed tavern shell after visual inspection from outside the room.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-018-calibrate-room-shell.md`

## Important Design Decisions

- Moved the paired outer wall instances onto the same wall centerlines as the interior wall instances:
  - back wall: `z = -4.0`
  - left wall: `x = -4.0`
  - right wall: `x = 4.0`
  - front wall sections: `z = 4.0`
- This removes the unnecessary visible gap between interior and exterior wall layers while keeping stone faces visible from both sides.
- Increased the Y scale of straight wall sections from `1.2` to `1.45`.
- Increased the Y scale of corner wall pieces from `1.1` to `1.45` so corners and straight walls reach a more consistent top height.
- Raised the ceiling from `y = 2.62` to `y = 3.2` so the chandelier does not pierce through the ceiling.
- Raised the door from `y = 0.0` to `y = 0.18` and slightly increased its Y scale.
- Added `entrance_door_outer`, a reversed door instance, so the door has a visible face from outside the tavern as well.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the scene now declares:
  - 24 models
  - 1 shared texture
  - 44 instances
- Verified that all instance model and texture references resolve to declared scene entries.
- Verified that all referenced model and texture files exist under `source/assets`.
- Command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required to visually verify that the paired walls do not show flickering and that the raised door aligns with the floor.

## Notes For Exam Review

- This step is scene composition work only. It uses per-instance transforms in `scene.json` and does not change the Vulkan framework.
- The paired wall and door instances are a simple explainable solution for one-sided-looking course assets.
- If exact overlap causes visible flickering on a specific GPU, the paired exterior faces can be offset by a very small amount in a later visual tuning step.
