# 2026-06-28 028 Revert Shell To Dungeon Palette

## Date

2026-06-28

## Goal

Temporarily replace the custom wall and door textures with the course asset texture atlas so the scene can be visually compared against the original dungeon material style.

## Changed Files

- `source/assets/scenes/scene.json`
- `0.Documentation/DevelopmentLog/2026-06-28-028-revert-shell-to-dungeon-palette.md`

## Important Design Decisions

- Kept the clean OBJ wall, ceiling, and door geometry unchanged.
- Changed only the texture references for:
  - `back_wall_clean`
  - `left_wall_clean`
  - `right_wall_clean`
  - `front_left_wall_clean`
  - `front_right_wall_clean`
  - `ceiling_clean`
  - `front_door_clean`
- All of those shell instances now use `dungeon_palette` from `assets/models/Textures.png`.
- Did not move or edit the five fixed wall candles.
- Did not restore the original `.MGCG` wall or door meshes because earlier testing showed those meshes include unwanted backplate geometry.

## Tested Or Verified

- Parsed `source/assets/scenes/scene.json` successfully with PowerShell `ConvertFrom-Json`.
- Verified that the wall, ceiling, and door shell instances now reference `dungeon_palette`.
- Verified that the five wall candle positions remain:
  - `wall_candle_backwall`: `[0, 3.35, -11.15]`
  - `wall_candle_FrontLeft`: `[-4.40, 2.15, -7.2]`
  - `wall_candle_FrontRight`: `[4.40, 2.15, -7.2]`
  - `wall_candle_RearLeft`: `[-4.40, 2.15, 1.8]`
  - `wall_candle_RearRight`: `[4.40, 2.15, 1.8]`
- No runtime visual check was completed in this step. Manual CLion run is required to judge whether the atlas material looks better than the custom wall and door textures.

## Notes For Exam Review

- This is a visual material comparison step, not a pipeline or shader change.
- If the atlas looks better, the custom `stone_wall_texture` and `wood_door_texture` entries can be removed later after confirming they are unused.
