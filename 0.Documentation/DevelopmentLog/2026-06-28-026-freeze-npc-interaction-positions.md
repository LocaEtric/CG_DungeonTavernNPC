# 2026-06-28 026 Freeze NPC Interaction Positions

## Date

2026-06-28

## Goal

Record the finalized NPC world positions so later interaction logic uses stable coordinates.

## Changed Files

- `0.Documentation/DevelopmentLog/2026-06-28-026-freeze-npc-interaction-positions.md`

## Important Design Decisions

- The user manually adjusted and approved the two NPC placements.
- These positions must be treated as fixed interaction anchors unless the user explicitly asks to move them.
- Later distance-based interaction code should use these world positions:
  - `npc_bartender`: `[0.0, 1.2, -10.2]`
  - `npc_door_greeter`: `[3.0, 1.05, 2.9]`
- Current visual models:
  - `npc_bartender` uses `ghost_cast`
  - `npc_door_greeter` uses `skeleton_helmet`
- The Y values are visual placement heights. For player proximity checks, using XZ-plane distance from the camera to each NPC is still recommended.

## Tested Or Verified

- Read the current `source/assets/scenes/scene.json` NPC instances.
- Confirmed current transforms:
  - `npc_bartender`: translate `[0, 1.2, -10.2]`, euler `[0, 0, 0]`, scale `[0.95, 0.95, 0.95]`
  - `npc_door_greeter`: translate `[3, 1.05, 2.9]`, euler `[0, -90, 0]`, scale `[1.1, 1.1, 1.1]`
- No code or scene changes were made in this step.

## Notes For Exam Review

- These two named NPC ids provide stable targets for the upcoming `Press E to interact` feature.
- The bartender should likely be the primary required interaction for the final demo.
- The door greeter can be a secondary interaction if time allows.
