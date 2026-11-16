# Project Management & Sprint Report — Snake Game

**Team SE_JACKFRUIT** · PES University · Software Engineering mini-project 2026
**Methodology:** Agile (Scrum-style) with a GitHub Projects board, GitHub Issues as the product backlog,
personal branches + pull requests, and GitHub Actions CI.

| Member | SRN | Role |
|---|---|---|
| Yele Charan | PES1UG25CS856 | SRS author · snake movement · test execution |
| Vrunda C | PES1UG24CS541 | Repo owner / team representative · Test Plan · food & scoring |
| Ved Mudkavi | PES1UG24CS522 | Architecture & design (SAD, UML) · collision detection · Scrum coordination |
| Tushar N | PES1UG24CS504 | RTM · integration & lifecycle · UI · CI/CD · backlog setup |

## Timeline (from repository history)

| Phase | Dates | Goal | Outcome |
|---|---|---|---|
| Inception | 1 Sep 2026 | Repo, branches, README, tech stack | Repo created by Vrunda; branches per member; README (Charan) |
| Phase 1 — Requirements & Design | 7 – 14 Oct | SRS, Test Plan, SAD, RTM, backlog (checkpoint 13 Oct) | All four documents in `docs/`; backlog issues #6–#12 created |
| **Sprint 1** | 19 – 23 Oct (work landed 23 – 29 Oct) | Working game core, GitHub Actions, reviewed PRs, 1-min demo | Movement, collision, food, controller + CI merged (#16–#20 closed) |
| **Sprint 2** | 26 – 30 Oct (work landed 29 Oct – 12 Nov) | Remaining features, testing, documentation, 2-min demo, freeze | ScoreManager + best score, FPS counter, execution addenda, SAD v1.1, 68→73 tests |
| Release | 16 – 17 Nov | CI/CD hardening, final docs, presentation | v1.0.0 baseline (this release) |

## Product backlog

| # | Item | Type | Owner | Requirements | Sprint | Status |
|---|---|---|---|---|---|---|
| 6 | Review the RTM against the SRS and Test Plan | Review | Tushar | all | Phase 1 | Done (23 Oct) |
| 7 | Resolve missing details in the Test Plan | Doc | Vrunda | — | Phase 1 | Done via execution addendum (environment, controls, versions recorded) — close issue |
| 8 | Review the SAD | Review | Ved | NFR-04 | Phase 1 | Done (31 Oct) |
| 9 | Review test cases TC-01 – TC-14 | Review | Vrunda, Charan | all | Phase 1 | Done (31 Oct) |
| 10 | Execute test cases and record results | Test | Charan | all | Sprint 2 | Done (31 Oct) |
| 11 | Final project integration review | Review | All | all | Sprint 2 | Open → close after final check |
| 12 | Prepare final submission | Release | All | all | Release | Open → close on submission |
| 16 | Keyboard control and snake movement | Story | Charan | FR-04–06, SR-01, SR-04 | Sprint 1 | Done |
| 17 | Food placement, growth and scoring | Story | Vrunda | FR-07–11 | Sprint 1→2 | Done |
| 18 | Wall and self-collision detection | Story | Ved | FR-12–14 | Sprint 1 | Done |
| 19 | Integrate playable game, display, lifecycle | Story | Tushar | FR-01–03, FR-14–18, SR-05 | Sprint 1 | Done |
| 20 | Build instructions and GitHub Actions | Story | Tushar | NFR-02, NFR-05, NFR-06 | Sprint 1 | Done |
| 21 | Record & upload Sprint 1 demo video (~1 min) | Deliverable | Tushar | — | Sprint 1 | **Open — still to do** |
| 22 | Record Sprint 2 demo (~2 min), freeze development | Deliverable | Tushar | — | Sprint 2 | **Open — still to do** |

### Epics
1. **Requirements & design** (#6–#9) 2. **Gameplay core** (#16–#18) 3. **Integration & lifecycle** (#19)
4. **DevOps** (#20, v1.0.0 CI/CD) 5. **Verification** (#10, #11) 6. **Release & demo** (#12, #21, #22)

## Velocity / burndown (by closed backlog items)

| Date | Items open (of 14) |
|---|---|
| 12 Oct | 14 |
| 23 Oct | 13 |
| 29 Oct | 10 |
| 31 Oct | 7 |
| 1 Nov | 5 (#7, #11, #12, #21, #22) |
| 17 Nov target | 0 |

## Pull requests & reviews
17 PRs were merged. Sprint planning asked for *reviewed* PRs, but most PRs were merged by their own author
without a recorded GitHub review; PR #23 (Ved) was merged by Tushar and PRs #5 and #29 (Ved) by Vrunda.
Issue #9 carries a written review comment from Charan. Vrunda's food/score module (4 commits, 31 Oct)
was pushed directly to `main`.
**Process improvement adopted for v1.0.0:** CODEOWNERS + PR template make a non-author review part of the
Definition of Done; enabling branch protection on `main` ("require 1 approval + passing CI") is recommended.

## Retrospective

| Went well | Didn't go well | Action |
|---|---|---|
| Clear module ownership mapped to backlog items | Late start on code (first real code 23 Oct) | Start a walking skeleton in Sprint 0 |
| Pure, SFML-free domain logic made testing easy (73 tests) | Self-merged PRs; one direct push to `main` | Branch protection, CODEOWNERS |
| CI caught environment drift (Ubuntu/SFML pin) | Demo videos (#21, #22) not uploaded on time | Record during sprint review |
| Documents cross-checked for ID consistency (FR/NFR/SR/TC) | Placeholder dates/versions in Test Plan for a while | Fill environment details at first build |

## Technical debt register

| Item | Impact | Suggested fix |
|---|---|---|
| `Snake` stores `sf::RectangleShape` (rendering) inside the domain model and lives in the global namespace | Movement tests need SFML; violates logic/presentation separation | Store `Body` of `Cell`s; draw in `main.cpp` |
| `src/main.cpp` is ~430 lines (input + rendering + HUD) | Harder to test and review | Extract a `Renderer` class |
| `assets/arial.ttf` is a proprietary font | Licence risk for public release | Replace with DejaVu Sans |
| "Increasing difficulty" from the original README not implemented | Planned feature dropped (scope change) | Requires SRS change request first |
| Snake movement tests use a hand-rolled harness, not GoogleTest | Inconsistent reporting (1 CTest entry for 6 checks) | Port to GoogleTest |

## Risk register

| Risk | Likelihood | Impact | Mitigation (status) |
|---|---|---|---|
| SFML version mismatch across machines | High | High | Documented versions, CI on Linux + macOS (done) |
| Team availability (travel, other courses) | High | Medium | Small backlog items, async PR workflow (done) |
| Integration bugs between modules | Medium | High | Integration tests through `GameController` (done) |
| Tail self-collision false positive | Medium | Medium | Departing-tail rule + TC-05 fixture tests (done) |
| Merge conflicts | Medium | Low | Pull `main` before push; per-module files (done) |
