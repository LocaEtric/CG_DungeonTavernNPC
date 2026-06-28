# 2026-06-28 008 First Tavern Asset Shortlist

## Goal

Select a small, understandable set of course-provided `.MGCG` assets for the first playable tavern scene, before editing `scene.json`.

## Changed Files

- `0.Documentation/DevelopmentLog/2026-06-28-008-first-tavern-asset-shortlist.md`

## Important Design Decisions

- Keep the first scene small enough to debug visually.
- Prefer assets that clearly communicate a tavern: floor, stone wall segments, door, tables, chairs, barrels, crates, cupboard/bookshelf, candles, bottles, and a simple placeholder NPC marker.
- Avoid the graveyard, torture, pumpkin, coffin, and large decorative sets for the first tavern pass because they make the theme less focused.
- Use one repeated render technique for all selected assets first. Extra pipelines or special materials can be added later only if needed.

## Selected Assets For First Scene

### Room Structure

- `floor_Mesh.7127.mgcg` / `floor_Mesh.7127.png`
- `tunnel.021_Mesh.196.mgcg` / `tunnel.021_Mesh.196.png`
- `tunnel.022_Mesh.4400.mgcg` / `tunnel.022_Mesh.4400.png`
- `tunnel.030_Mesh.7968.mgcg` / `tunnel.030_Mesh.7968.png`
- `door.002_Mesh.7405.mgcg` / `door.002_Mesh.7405.png`

### Tavern Furniture

- `table.001_Mesh.7007.mgcg` / `table.001_Mesh.7007.png`
- `table.015_Mesh.7354.mgcg` / `table.015_Mesh.7354.png`
- `chair.003_Mesh.6582.mgcg` / `chair.003_Mesh.6582.png`
- `chair.005_Mesh.7153.mgcg` / `chair.005_Mesh.7153.png`
- `barrel.001_Mesh.4453.mgcg` / `barrel.001_Mesh.4453.png`
- `barrel.002_Mesh.4450.mgcg` / `barrel.002_Mesh.4450.png`
- `box.002_Mesh.899.mgcg` / `box.002_Mesh.899.png`
- `box.006_Mesh.216.mgcg` / `box.006_Mesh.216.png`
- `cupboard.001_Mesh.6553.mgcg` / `cupboard.001_Mesh.6553.png`

### Small Props

- `jug.003_Mesh.902.mgcg` / `jug.003_Mesh.902.png`
- `jug.009_Mesh.220.mgcg` / `jug.009_Mesh.220.png`
- `prop.030_Mesh.7394.mgcg` / `prop.030_Mesh.7394.png`
- `prop.035_Mesh.6860.mgcg` / `prop.035_Mesh.6860.png`
- `prop.044_Mesh.6555.mgcg` / `prop.044_Mesh.6555.png`

### Light Visuals

- `light.010_Mesh.6200.mgcg` / `light.010_Mesh.6200.png`
- `light.012_Mesh.4744.mgcg` / `light.012_Mesh.4744.png`
- `light.017_Mesh.470.mgcg` / `light.017_Mesh.470.png`

### NPC Placeholder

- `prop.042_Mesh.7896.mgcg` / `prop.042_Mesh.7896.png`

This globe-like prop can mark the NPC position temporarily. It should be replaced by a better character-like model or a simple composed NPC later if a suitable character asset is found.

## Tested Or Verified

- Asset names were checked from `source/assets/models`.
- Preview thumbnails were generated only in a temporary directory and were not added to the project.
- No source, shader, scene, or asset file behavior was changed.

## Notes For Exam Review

- The first scene should demonstrate model loading and texture mapping with a controlled number of models.
- The NPC placeholder is intentionally simple so interaction logic can be implemented before spending time on character art.
- The next implementation step should edit `source/assets/scenes/scene.json` to instantiate these selected assets into a readable tavern layout.
