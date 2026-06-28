# Agent Working Agreement

This file must be read at the start of every agent session before changing the project.

## Communication

- Reply to the user in Chinese.
- Write source code, identifiers, commit messages, and code comments in English.
- Markdown project notes may use Chinese unless the user asks otherwise.

## Course And Skeleton Rules

- This project is the Computer Graphics final project topic: Dungeon Tavern NPC.
- The project must use C++ and Vulkan with the provided course skeleton.
- Do not modify files under `source/include`.
- Do not modify the provided framework modules such as `Starter.hpp`, `Scene.hpp`, `TextMaker.hpp`, or collider framework files.
- Project-specific logic should be implemented in `source/src`, `source/shaders`, `source/assets`, CMake configuration, and project documentation.
- Shaders used for the project must be written and understood by the team.
- The implementation must keep explicit, explainable sections for:
  - mesh/model loading or scene setup
  - texture loading and mapping
  - vertex formats
  - uniform buffers and descriptor layouts
  - graphics pipelines and render passes
  - command buffer population and draw calls
  - camera/view/projection transforms
  - lighting and interaction logic

## Code Style

- Keep code simple, readable, and easy to explain during the exam.
- Prefer small, direct functions over clever abstractions.
- Add comments only when they help explain non-obvious logic.
- Comments in code must be written in English.
- Avoid large unrelated refactors.
- Preserve the course skeleton structure unless a change is necessary for the project.

## Documentation Discipline

- Every meaningful implementation step must be recorded in a separate markdown file under `0.Documentation/DevelopmentLog/`.
- Each step document should include:
  - date
  - goal
  - changed files
  - important design decisions
  - how the change was tested or verified
  - notes for exam review
- Keep `0.Documentation/ProjectPlan.md` updated when the project scope or milestone order changes.
- Do not rely only on chat history for important decisions.

## Build And Asset Rules

- Do not treat files under `cmake-build-debug` as source of truth.
- Do not document or depend on generated build artifacts as if they were project sources.
- Keep assets organized under `source/assets`.
- Record external or course-provided asset choices in documentation.
- Course-provided `.MGCG` assets are only for this Computer Graphics final project.

## Verification

- After code or shader changes, build the project when practical.
- If a runtime feature is changed, run the application when practical and record the result.
- If verification cannot be completed, document the reason clearly in the development log and final response.
- If the local compiler or build tool repeatedly returns unclear errors while the user can build and run successfully in CLion, do not keep retrying the same build loop. Stop after a reasonable check, ask the user to manually build/run, and record manual verification status in the development log.
