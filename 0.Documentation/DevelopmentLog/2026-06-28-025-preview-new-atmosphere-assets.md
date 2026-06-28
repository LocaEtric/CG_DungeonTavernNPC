# 2026-06-28 025 Preview New Atmosphere Assets

## Date

2026-06-28

## Goal

Register and preview three newly added atmosphere assets: cast, decoration, and bones.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-025-preview-new-atmosphere-assets.md`

## Important Design Decisions

- Registered three newly available `.MGCG` assets:
  - `ghost_cast` from `assets/models/cast_Mesh.6268.mgcg`
  - `stone_statue` from `assets/models/decoration.017_Mesh.299.mgcg`
  - `skeleton_bones` from `assets/models/bones.001_Mesh.7532.mgcg`
- Added three preview instances without moving the user-tuned furniture, bar, table, chair, and NPC placements:
  - `preview_ghost_cast` near the right rear side of the room
  - `preview_stone_statue` near the left rear side of the room
  - `preview_skeleton_bones` near the front-left entry area
- Used the existing `dungeon_palette` texture for all three preview assets so the descriptor setup remains simple.
- Kept the instance ids prefixed with `preview_` to make it clear that these are temporary visual placement tests.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the scene now declares:
  - 32 models
  - 3 textures
  - 43 instances
- Verified that all scene model and texture references resolve to existing project files.
- Command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required for visual verification.

## Notes For Exam Review

- These assets add dungeon atmosphere but are not part of the core NPC interaction logic.
- The preview ids can be renamed or repositioned later after visual inspection.
