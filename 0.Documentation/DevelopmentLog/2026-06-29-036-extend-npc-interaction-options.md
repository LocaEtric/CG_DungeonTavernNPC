# 2026-06-29 036 Extend NPC Interaction Options

## Date

2026-06-29

## Goal

Improve the two NPC interactions so the bartender can be triggered from in front of the counter, can offer drink choices, and the greeter can reset the registration state through checkout.

## Changed Files

- `source/src/main.cpp`
- `0.Documentation/DevelopmentLog/2026-06-29-036-extend-npc-interaction-options.md`

## Important Design Decisions

- Added separate interaction radii for the two NPCs:
  - greeter: `2.0`
  - bartender: `3.4`
- Switched NPC proximity checks to horizontal XZ distance so camera height does not affect whether the player can interact.
- Added a simple bartender menu using number keys:
  - `1` for ale
  - `2` for red wine
  - `3` for water
- Supported both top-row number keys and keypad number keys.
- Added `drinkChoiceKeyWasPressed` to debounce drink selection.
- Added a greeter checkout path: after registration, pressing `E` at the greeter cancels registration and clears bartender order/menu state.
- Kept the interaction logic as a simple state machine instead of adding a full dialogue system.
- Did not modify framework files or files under `source/include`.

## Tested Or Verified

- Local code review confirmed the bartender uses the larger `bartenderInteractionRadius`.
- Local code review confirmed the greeter checkout path resets:
  - `registered`
  - `orderCompleted`
  - `bartenderMenuOpen`
  - `selectedDrink`
- Command-line build was not run because this PowerShell environment does not expose `cmake`, `ninja`, or `mingw32-make`.
- Manual CLion build/run verification is required to confirm:
  - bartender dialogue triggers from in front of the counter
  - pressing `E` opens the bartender drink menu
  - pressing `1`, `2`, or `3` selects a drink once
  - returning to the greeter after registration allows checkout
  - checkout resets the bartender interaction flow

## Notes For Exam Review

- The interaction logic remains distance-based and easy to explain.
- The drink menu demonstrates a small interactive state machine without needing text input parsing.
- Checkout is a useful reset path for repeating the final demo during the exam.
