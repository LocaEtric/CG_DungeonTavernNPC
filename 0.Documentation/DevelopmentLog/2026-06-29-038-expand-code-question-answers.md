# 2026-06-29 038 Expand Code Question Answers

## Date

2026-06-29

## Goal

Expand `Code Questions.md` into a detailed exam preparation document. The goal is to answer each listed code question from three levels:

- concept definition
- graphics/Vulkan principle
- concrete project implementation

The document also records common difficulties and the solutions used in this project.

## Changed Files

- `0.Documentation/Code Questions.md`
- `0.Documentation/DevelopmentLog/2026-06-29-038-expand-code-question-answers.md`

## Important Design Decisions

- Kept the original question categories:
  - Vertex Format
  - Uniform Buffers
  - Descriptor Layouts
  - Pipeline
  - Scene Loading
  - Command Buffer And Draw Calls
  - Camera
  - Projection
  - Lighting
  - NPC Interaction
  - Text Output
  - Shader
- Marked each question as completed with `[x]`.
- Used a consistent answer structure:
  - what it is
  - principle
  - project implementation
  - difficulties and solutions
  - exam speaking template
- Focused explanations on actual project code instead of generic textbook-only answers.
- Did not modify C++ source files, shader files, assets, CMake, or framework code.

## Tested Or Verified

- Reviewed `source/src/main.cpp`.
- Reviewed `source/shaders/tavern_lit.vert`.
- Reviewed `source/shaders/tavern_lit.frag`.
- Reviewed `0.Documentation/ExamReview.md`.
- Reviewed `0.Documentation/DevelopmentLog/2026-06-29-037-document-shader-pipeline.md`.
- Verified that all original questions in `Code Questions.md` have been expanded and marked as answered.

No build or runtime verification was required because this step only changes documentation.

## Notes For Exam Review

- `Code Questions.md` is now the most detailed oral-exam preparation document.
- `ExamReview.md` remains the quick overview.
- `2026-06-29-037-document-shader-pipeline.md` remains the focused shader/pipeline implementation record.
- Recommended study order:
  1. `ExamReview.md`
  2. `Code Questions.md`
  3. shader files and `main.cpp`
  4. development logs for specific implementation history
