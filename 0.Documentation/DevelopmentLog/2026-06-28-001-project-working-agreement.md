# 2026-06-28 001 Project Working Agreement

## Goal

Create persistent project rules for future agent sessions and record the initial Dungeon Tavern NPC implementation plan.

## Changed Files

- `AGENTS.md`
- `0.Documentation/ProjectPlan.md`
- `0.Documentation/DevelopmentLog/README.md`
- `0.Documentation/DevelopmentLog/2026-06-28-001-project-working-agreement.md`

## Design Decisions

- Use `AGENTS.md` at the repository root as the first file every future agent session should read.
- Keep agent replies in Chinese.
- Keep code, identifiers, and code comments in English.
- Do not modify `source/include` because the course starter framework must remain compatible with the professor's version.
- Record each meaningful implementation step as a separate markdown document for review and exam preparation.
- Keep the first project target small: render a minimal scene, then add first-person camera, then add NPC interaction.

## Verification

- Documentation-only change.
- No build or runtime verification required.

## Exam Review Notes

- The project scope is intentionally simple and explainable.
- The required exam topics are covered by the planned milestones: model loading, texture mapping, camera, shaders, lighting, Vulkan pipeline flow, and interaction logic.

