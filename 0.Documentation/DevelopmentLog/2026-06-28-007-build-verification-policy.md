# 2026-06-28 007 Build Verification Policy

## Goal

Record the project-specific verification rule requested by the user: when the agent's local build command repeatedly fails with unclear compiler or toolchain errors, but the user can build and run successfully in CLion, the agent should stop retrying the same build loop and ask for manual verification instead.

## Changed Files

- `AGENTS.md`
- `0.Documentation/DevelopmentLog/2026-06-28-007-build-verification-policy.md`

## Important Design Decisions

- Added the rule under the existing verification section because it affects how future code and shader changes are validated.
- Kept the rule narrow: it applies to repeated unclear local compiler or build tool failures, especially when the same project is known to build and run on the user's CLion setup.
- Manual user build/run results are acceptable verification evidence when local agent-side build output is unreliable.

## Tested Or Verified

- Documentation-only change.
- No code, shader, CMake, or asset behavior was changed.

## Notes For Exam Review

- This note is for development workflow only.
- It does not change the project implementation or course feature scope.
