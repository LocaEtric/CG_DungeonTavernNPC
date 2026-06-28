# 2026-06-28 024 Add Two NPC Placeholders

## Date

2026-06-28

## Goal

Add two NPC placeholders to the tuned tavern layout without moving the user-adjusted furniture and bar objects.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-024-add-two-npc-placeholders.md`

## Important Design Decisions

- Preserved the user-tuned positions for:
  - `bar_counter_table`
  - `bar_support_box_fl`
  - `bar_support_box_fr`
  - `bar_support_box_bl`
  - `bar_support_box_br`
  - `back_cupboard_L`
  - `back_locker_R`
  - `left_round_table`
  - `left_table_chair_a`
  - `left_table_chair_b`
  - `left_table_chair_c`
  - `right_bench_table`
  - `right_long_table_chair_a`
  - `right_long_table_chair_b`
  - `front_porch_table`
  - `right_porch_table_chair`
- Removed the old single `npc_placeholder` instance and replaced it with two named NPC instances:
  - `npc_bartender`
  - `npc_door_greeter`
- Reused the existing `npc_marker` model for both NPCs because the project currently has no full-body character model registered.
- Placed `npc_bartender` behind the bar at `[0.0, 1.46, -10.15]`, facing the player side of the bar.
- Placed `npc_door_greeter` near the door-side porch chair at `[3.55, 1.25, 2.0]`, angled toward the entry area.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the scene now declares:
  - 29 models
  - 3 textures
  - 40 instances
- Verified that all scene model and texture ids resolve.
- Checked that the user-listed tuned furniture and bar object transforms remained unchanged after adding NPCs.
- Command-line build was not completed because the current PowerShell environment does not expose `cmake`. Manual CLion build/run is required for visual verification.

## Notes For Exam Review

- The two named NPC instances create explicit targets for later interaction logic.
- The bartender should become the first interaction target for `Press E to interact`.
- The door greeter can be used later for a second prompt/dialogue if time allows.
- The visual model can be replaced later without changing interaction ids or world positions.
