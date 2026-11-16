# Changelog

All notable changes to the Snake Game are recorded here.
The project follows [Semantic Versioning](https://semver.org/) (MAJOR.MINOR.PATCH) and the
[Keep a Changelog](https://keepachangelog.com/) format. A release is cut by pushing a tag
`vMAJOR.MINOR.PATCH`; `.github/workflows/release.yml` then builds, tests and publishes it.

## [1.0.0] — 2026-11-17 (final submission baseline)

### Added
- **Continuous Delivery**: tag-triggered release workflow that runs the full test suite as a gate,
  packages Linux (x86_64) and macOS (arm64) builds with CPack, generates an SPDX **SBOM** and
  SHA-256 checksums, and publishes a GitHub Release.
- **CI quality gates** (`ci.yml`): strict-warning build (`-Wall -Wextra -Wpedantic -Werror`),
  **ASan + UBSan** run of all tests, **cppcheck** static analysis, **security scan** for SR-02 /
  SR-03 (`scripts/security_scan.sh`), macOS build, JUnit test report, nightly Linux artifact.
- CMake options `SNAKE_ENABLE_WARNINGS`, `SNAKE_WARNINGS_AS_ERRORS`, `SNAKE_ENABLE_SANITIZERS`,
  install rules and CPack packaging.
- Dependabot for GitHub Actions, CODEOWNERS, PR template (Definition of Done), issue templates
  (user story, bug report), `.clang-format`, `.editorconfig`.
- Docs: `RTM.csv` / `RTM.md`, Software Architecture Diagram, UML Component Diagram, Use Case
  Diagram, CI/CD pipeline diagram, `CI_CD.md`, `CODING_STANDARDS.md`, `Sprint_Report.md`,
  `SBOM.md`, `CONTRIBUTING.md`, `STUDY_GUIDE.md`, docs index.

### Fixed
- `tests/snake_test.cpp` used `assert()`, which is compiled out in Release builds (`NDEBUG`), so the
  movement tests silently checked nothing in CI. Replaced with an always-on `CHECK` macro; verified that a
  deliberately wrong expectation now fails the test.

### Changed
- RTM: SR-02 and SR-03 moved from *In Progress* to *Pass* on the strength of the automated
  forbidden-API scan; TC-07 / TC-13 sanitizer sub-steps now run automatically in CI.
- README test counts updated (73 automated tests across 5 targets).

## [0.3.0] — 2026-11-07 (Sprint 2 complete)
- ScoreManager with session best score shown in HUD and end screens (Vrunda).
- FPS counter for performance testing NFR-01 (Charan).
- macOS font packaging and final collision evidence (Ved).
- SRS v1.1 execution update, Test Plan execution addendum, SAD v1.1 (team).

## [0.2.0] — 2026-10-29 (Sprint 1 complete)
- Snake movement with reversal latch and unit tests (Charan).
- Grid-based wall and self-collision detection with departing-tail rule (Ved).
- Food placement on free cells and scoring (Vrunda).
- GameController state machine, SFML window/HUD, start/restart, GitHub Actions CI (Tushar).

## [0.1.0] — 2026-10-13 (Phase 1 — requirements & design baseline)
- SRS v1.0 (Charan), Software Test Plan v1.0 (Vrunda), SAD + UML diagrams (Ved), RTM (Tushar).
- Product backlog created on GitHub Projects.
