# 2026-06-28 009 Prune Model Assets

## Goal

Reduce `source/assets/models` to only the assets selected for the first tavern scene, keeping each selected `.MGCG` model and its matching `.png` preview/texture file.

## Changed Files

- `source/assets/models`
- `0.Documentation/DevelopmentLog/2026-06-28-009-prune-model-assets.md`

## Important Design Decisions

- Kept only the 23 selected asset pairs from the first tavern shortlist.
- Removed unrelated graveyard, torture, pumpkin, coffin, fence, stone, extra prop, extra tunnel, and extra furniture assets to make the project easier to navigate and submit.
- Kept both `.mgcg` and `.png` files for each selected asset because the scene loader needs model files and the project needs matching texture/preview files.
- Did not modify generated build artifacts under `cmake-build-debug`.

## Kept Assets

- `floor_Mesh.7127.mgcg` / `floor_Mesh.7127.png`
- `tunnel.021_Mesh.196.mgcg` / `tunnel.021_Mesh.196.png`
- `tunnel.022_Mesh.4400.mgcg` / `tunnel.022_Mesh.4400.png`
- `tunnel.030_Mesh.7968.mgcg` / `tunnel.030_Mesh.7968.png`
- `door.002_Mesh.7405.mgcg` / `door.002_Mesh.7405.png`
- `table.001_Mesh.7007.mgcg` / `table.001_Mesh.7007.png`
- `table.015_Mesh.7354.mgcg` / `table.015_Mesh.7354.png`
- `chair.003_Mesh.6582.mgcg` / `chair.003_Mesh.6582.png`
- `chair.005_Mesh.7153.mgcg` / `chair.005_Mesh.7153.png`
- `barrel.001_Mesh.4453.mgcg` / `barrel.001_Mesh.4453.png`
- `barrel.002_Mesh.4450.mgcg` / `barrel.002_Mesh.4450.png`
- `box.002_Mesh.899.mgcg` / `box.002_Mesh.899.png`
- `box.006_Mesh.216.mgcg` / `box.006_Mesh.216.png`
- `cupboard.001_Mesh.6553.mgcg` / `cupboard.001_Mesh.6553.png`
- `jug.003_Mesh.902.mgcg` / `jug.003_Mesh.902.png`
- `jug.009_Mesh.220.mgcg` / `jug.009_Mesh.220.png`
- `prop.030_Mesh.7394.mgcg` / `prop.030_Mesh.7394.png`
- `prop.035_Mesh.6860.mgcg` / `prop.035_Mesh.6860.png`
- `prop.044_Mesh.6555.mgcg` / `prop.044_Mesh.6555.png`
- `light.010_Mesh.6200.mgcg` / `light.010_Mesh.6200.png`
- `light.012_Mesh.4744.mgcg` / `light.012_Mesh.4744.png`
- `light.017_Mesh.470.mgcg` / `light.017_Mesh.470.png`
- `prop.042_Mesh.7896.mgcg` / `prop.042_Mesh.7896.png`

## Tested Or Verified

- Before deletion, the resolved target path was checked to be inside the project workspace and exactly under `source/assets/models`.
- Before deletion, all 46 keep files were confirmed to exist.
- Deleted 708 non-selected files from `source/assets/models`.
- No code or shader build was run because this was an asset cleanup step only.

## Notes For Exam Review

- The project now contains a focused asset set for a tavern scene instead of a large unused asset dump.
- The next scene composition step can load every remaining model intentionally through `source/assets/scenes/scene.json`.
