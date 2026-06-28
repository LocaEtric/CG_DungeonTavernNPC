# 2026-06-28 011 Fix Asset Copy Target

## Goal

Fix the issue where editing `source/assets/scenes/scene.json` did not change the runtime scene after CLion reported `ninja: no work to do`.

## Changed Files

- `CMakeLists.txt`
- `cmake-build-debug/assets` was manually synchronized for local runtime testing only.
- `0.Documentation/DevelopmentLog/2026-06-28-011-fix-asset-copy-target.md`

## Important Design Decisions

- The application loads runtime assets from the build output directory, for example `cmake-build-debug/assets/scenes/scene.json`.
- The previous asset copy command was attached as `POST_BUILD` on the executable target. It only ran when the executable target actually rebuilt or relinked.
- Scene JSON and asset edits do not necessarily trigger executable rebuilding, so Ninja could correctly say `no work to do` while the runtime asset copy stayed stale.
- Replaced the `POST_BUILD` copy with a dedicated `copy_assets` target that is part of `ALL`.
- `copy_assets` now removes the old runtime asset directory before copying, so deleted source assets do not remain as stale files under the build directory.
- The main executable target depends on `copy_assets`, keeping assets available before normal runs.

## Tested Or Verified

- Confirmed stale runtime scene before the fix:
  - build directory scene: 1 model, 1 texture, 1 instance
  - source scene: 23 models, 23 textures, 33 instances
- Manually synchronized `source/assets` to `cmake-build-debug/assets` once for the current run directory.
- Verified synchronized runtime scene:
  - build directory scene: 23 models, 23 textures, 33 instances
  - build directory model files: 46

## Notes For Exam Review

- This is a build-system support fix, not a graphics feature.
- The important point is that runtime asset files must match source asset files when testing scene composition.
