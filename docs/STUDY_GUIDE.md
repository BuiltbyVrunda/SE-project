# Snake Game — Study & Viva Guide

**Team SE_JACKFRUIT** · Software Engineering mini-project · PES University · Presentation: 17 Nov 2026
Repo: `github.com/BuiltbyVrunda/SE-project`

> How to use this: read Part 1 (the 60-second pitch) until you can say it without notes, then your own row
> in Part 2, then skim Part 3 for vocabulary, then drill Part 4. Every answer here is based on the actual
> repository, so you can point to a file while answering.

---

## Part 1 — The project in 60 seconds

A single-player **Snake game in C++17 with SFML 2**, built with **CMake** and tested with **GoogleTest**.
The player steers with the arrow keys, eats food (+10 points, the snake grows by one), and the game ends on
hitting a wall or itself; **R** restarts and the session best score is kept.

The architecture is **layered**:
- **Presentation** (`main.cpp`: SFML window, input, HUD)
- **Application** (`GameController`: a state machine Ready → Playing → GameOver/RoundComplete)
- **Domain** (`Snake`, `FoodManager`, `ScoreManager`, `CollisionDetector`): pure rules, most of them with no
  SFML, so they are unit-testable without a window.

The process followed **Agile/Scrum**:
- IEEE-style **SRS** (29 requirements: 18 FR, 6 NFR, 5 SR) → **Test Plan** (14 test cases, IEEE 829) →
  **SAD** with UML component and sequence diagrams → **RTM** linking every requirement to a test.
- Backlog on GitHub Projects, two sprints, one branch per person, pull requests.
- **73 automated tests**, and a **CI/CD** pipeline: GitHub Actions build/test, sanitizers, static
  analysis, a security scan and macOS build on every push; tagged releases are packaged with an SBOM.

**Key numbers to remember:** 4 members · 29 requirements · 14 test cases · 73 automated tests ·
5 test targets · 5 CI jobs · 17 merged PRs · 2 sprints (19–23 Oct, 26–30 Oct) · grid 40×30 cells of
20 px (800×600 window) · 0.15 s per move · 60 FPS cap · 10 points per food · start = 3 segments at (5,5)
moving right.

---

## Part 2 — Who did what

### Team

| Member | SRN | One-line role |
|---|---|---|
| **Yele Charan** | PES1UG25CS856 | Requirements (SRS) · snake movement · test execution · most commits |
| **Vrunda C** | PES1UG24CS541 | Repo owner & team representative · Test Plan · scoring & best score |
| **Ved Mudkavi** | PES1UG24CS522 | Architecture & design (SAD + UML) · collision detection · de-facto Scrum master |
| **Tushar N** | PES1UG24CS504 | RTM · integration (GameController, UI, food placement) · GitHub Actions CI |

Agreed functional split (WhatsApp, 27 Sep / 26 Oct): **Charan = movement · Vrunda = food + scoring ·
Ved = collision · Tushar = lifecycle / integration / CI.**

### Detailed contributions (from git history, PRs, issues and the group chat)

#### Yele Charan: requirements and movement
- **Project setup:** created the WhatsApp group, the first commit and the `charan`/`main` branches; wrote the
  first README draft (tech stack, planned features, Agile approach). (1 Sep)
- **SRS v1.0** (`docs/Snake_Game_SRS.pdf`, PR #4, 12 Oct): FR-01…18, NFR-01…06, SR-01…05, security
  objectives, use-case diagram, initial RTM. Fixed the SRN and spelling errors the team pointed out.
  Later added the **SRS v1.1 / Execution Update** and the **Test Plan Execution Update** PDFs (7 Nov).
- **Code:**
  - Initial `Snake` class skeleton (PR #3, 7 Oct).
  - **Snake movement**, direction changes and **reversal rejection with an input latch**
    (`directionChangedThisStep`) (PR #24).
  - Expanded the movement tests (PR #25) and wrote the movement validation report (PR #26).
  - Snake appearance and growth support (PR #27).
  - **FPS counter** for the NFR-01 performance test (PR #30).
  - A large integration update of `main.cpp`; most of today's `main.cpp` lines are Charan's per
    `git blame`.
- **Testing:** owned issue #10 (execute TC-01…TC-14, record results), reviewed the test cases on issue #9,
  and captured the 68/68 CTest evidence screenshot on Windows.
- Most active committer (16 non-merge commits).

#### Vrunda C: Test Plan and scoring
- **Repository owner:** created `BuiltbyVrunda/SE-project`, set `main` as the default branch, added the
  collaborators, filled the project registration form, and is the **team representative** on all documents.
- **Software Test Plan** (`docs/Test_Plan_SnakeGame 3.pdf`, IEEE 829 structure, three revisions on 12 Oct):
  scope, test items, features, approach (unit/integration/system/acceptance), environment, schedule,
  severity table, TC-01…TC-14. Incorporated review fixes: the self-collision test against a non-tail
  segment, keeping food off the TC-05 route, and aligning the integration flow with the SAD.
- **Code (31 Oct):**
  - `ScoreManager` module: 10 points per food, foods eaten, session **best score** kept across restart,
    non-positive values fall back to the default.
  - Best score shown in the HUD and on the game-over/win screens.
  - **29 tests:** `tests/food_score_test.cpp` (19: placement over 500 seeds, last free cell,
    BoardFull, invalid board, scoring rules) and `tests/food_score_integration_test.cpp` (10: growth by
    exactly one, single consumption, 5-food score, replacement food, best score across restart).
  - `reports/food_scoring_validation.md`.
- **Reviews and merges:** merged Ved's PRs #5 and #29 and the document PR #13.
- Note: the food *placement* code (`FoodManager`) first landed inside Tushar's integration commit.
  Vrunda's work is the scoring layer plus the food tests that verify placement.

#### Ved Mudkavi: architecture and collisions
- **Software Architecture & Design document** (`docs/SAD_SnakeGame.docx`, v1.0 on 12 Oct, v1.1 on 7 Nov):
  goals and constraints, stakeholders, **component UML diagram**, two **sequence diagrams** (gameplay/collision
  and food consumption), architecture pattern and rationale, tech stack, risk table, traceability, security
  architecture, interfaces, error handling.
- **Code:**
  - `CollisionDetector`: a **pure, side-effect-free** check of the proposed next head *before* moving.
    It detects the boundary (all 4 walls) and the self-body, applies the **departing-tail rule**, and
    rejects invalid input (empty body, non-positive bounds throw `std::invalid_argument`).
  - **23 GoogleTest tests**, several of them parameterised (`TEST_P`) (PR #23).
  - Collision integration validation, the SAD review against the integrated game, and macOS font
    packaging (PR #29).
- **Coordination:** reviewed the SRS and Test Plan in chat (caught the SRN errors, the wrong
  responsibilities, undefined "High severity", missing sprint dates), **created backlog issues #16–#22**
  (one per member with acceptance criteria), set up the sprint board for two sprints, and kept chasing the
  Sprint 1/2 deliverables.

#### Tushar N: integration, RTM and CI
- **RTM** (`docs/RTM_SnakeGame.xlsx`): 29 requirements mapped to TC-01…TC-14, plus an integration-review
  checklist and a coverage summary. **Created backlog issues #6–#12** (Phase 1 review and submission tasks).
- **Code (PR #28, 29 Oct):**
  - **`GameController`** state machine: start, restart, fixed-timestep `update(dt)`, and the
    ordered `step()` (check collision first, then move).
  - SFML window, HUD and start/game-over screens in `main.cpp`.
  - The first **`FoodManager`** (free-cell placement, BoardFull).
  - `tests/game_integration_test.cpp`: the 12 required scenarios plus a four-wall integration test.
- **CI:** wrote `.github/workflows/ci.yml` (closes #20) and fixed it by **pinning the runner to Ubuntu
  22.04** for SFML 2 compatibility.
- **Integration:** merged Ved's collision PR #23 into the integrated build; wrote the
  `reports/integration_validation.md` and `docs/demo_presentation_plan.md`.
- **Still open:** demo videos #21 (Sprint 1, ~1 min) and #22 (Sprint 2, ~2 min).

### v1.0.0 release hardening (16 Nov, final pre-submission pass)
- CI expanded to 5 jobs (strict warnings, ASan+UBSan, cppcheck, security scan, macOS); a tag-triggered
  **CD release** with packages, SBOM and checksums.
- Fixed `snake_test.cpp`, whose `assert` was disabled in Release so it checked nothing in CI.
- Added: RTM.csv/RTM.md, the architecture/component/use-case/pipeline diagrams, CI_CD.md, the
  Sprint Report, Coding Standards, SBOM, CONTRIBUTING, CHANGELOG, PR/issue templates, CODEOWNERS and
  Dependabot.
- Whoever commits this should be able to explain it (see Part 4 §7).

---

## Part 3 — SE topics applied (mapped to the syllabus)

### Unit 1: Introduction, Requirements Engineering, Testing introduction

| Topic | Where it shows up in this project |
|---|---|
| **SDLC / Agile / Scrum** | Requirements → design → implementation → test → release, run as 2 sprints with a backlog, sprint goals, review deliverables (demo videos) and a retrospective (`docs/Sprint_Report.md`). |
| **Requirements elicitation & analysis** | Features came from the course brief and team discussion. The first README listed "increasing difficulty", which was later deferred (scope control). |
| **Functional requirements** | FR-01…FR-18 (start, steer, reject reversal, food, grow, score, collisions, game over, restart, safe close). |
| **Non-functional requirements** | NFR-01 ≥30 FPS · NFR-02 usability (controls documented) · NFR-03 reliability (10 restart cycles) · NFR-04 maintainability (modules) · NFR-05 compatibility · NFR-06 documentation. |
| **Security requirements** | SR-01 ignore unsupported keys · SR-02 no OS commands · SR-03 no unrelated file access · SR-04 validate direction against state · SR-05 safe shutdown. |
| **SRS (IEEE 830/29148 style)** | `Snake_Game_SRS.pdf`: purpose, scope, definitions, product perspective, user classes, constraints, interfaces, requirements, use-case model, RTM. |
| **UML use-case diagram, personas** | SRS §7 and `docs/diagrams/Use_Case_Diagram.png`. Actors: Player, Developer/Tester. «include» Validate direction, «extend» Show Game Over. |
| **Requirement Traceability Matrix** | `RTM.md`/`RTM.csv`/`.xlsx`: **forward** traceability (requirement → test → evidence) and **backward** traceability (test → requirement). |
| **Requirement change management** | SRS v1.0 → v1.1 execution update; SAD v1.0 → v1.1; "difficulty" feature deferred pending an SRS change. |
| **Test planning / security validation plan** | IEEE 829 Test Plan with TC-14 as the security validation case; sanitizer steps in TC-07/TC-13. |

### Unit 2: Project management, architecture, design, quality

| Topic | Where |
|---|---|
| **Project management, roles, teamwork** | Owner per module, team representative, coordination over chat, GitHub Projects board. |
| **Scheduling, Gantt, monitoring** | Sprint dates (19–23 Oct, 26–30 Oct), burndown table, open/closed issues as progress tracking. |
| **Risk management** | Risk table in the SAD (§3.7) and the risk register in the Sprint Report: SFML mismatch, input bursts, tail false-positive, full board, timing drift, stale restart state. |
| **Architecture: views and abstraction** | Logical view (component diagram), process view (sequence diagrams), development view (CMake targets), deployment (single desktop process). |
| **Architecture style** | **Layered** (presentation → application → domain) and modular; explicitly *not* microservices or client-server (no need for a network; rationale in SAD §3.5). |
| **Design patterns** | **State pattern / state machine** (GameState), **Controller/Mediator** (GameController coordinates modules that don't know each other), **Dependency Injection** (`std::mt19937&` passed into `FoodManager::place` for deterministic tests), **Value objects** (`Cell`, `BoardBounds`), **Facade** (GameController is the single API the UI calls). |
| **Modularity, cohesion, coupling** | Each class has one responsibility (high cohesion). The domain classes depend only on `grid_types.h` (low coupling). Each is its own CMake library. |
| **API design** | Small headers, `[[nodiscard]]` queries, `enum class` results (`PlacementResult`, `CollisionType`), and `std::optional<Cell>` for "maybe no food". |
| **Error handling** | Invalid bounds → `std::invalid_argument` (collision) or `PlacementResult::InvalidBounds` (food); full board → `BoardFull` → `RoundComplete` state; font fallback chain. |
| **Usability engineering** | On-screen instructions, Game Over reason (wall vs self), final and best score, R to restart, 60 FPS, NFR-02 test with an external reviewer. |
| **Security architecture** | No network, no file writes, no command execution; input is validated against the game state. |
| **Software quality, technical debt** | Technical-debt register (Snake holds SFML shapes, a large `main.cpp`, a proprietary font, the assert-based test harness). |
| **Test-driven development** | Tests were written per module alongside code, with acceptance criteria in each issue; integration scenarios defined before integration. |

### Unit 3: AI, implementation, SCM, DevOps

| Topic | Where |
|---|---|
| **AI for SE / vibe coding** | Team members used ChatGPT/Claude/Codex to draft and review documents (visible in chat). Be honest: the AI drafted, and humans checked against the code and fixed the errors (e.g. wrong SRNs and responsibilities in the AI draft). |
| **Language choice** | C++17 for performance and control, SFML for simple 2D graphics/input; CMake for cross-platform builds. |
| **Defensive & secure coding** | Bounds checks before indexing (`occupied[y*cols+x]` only when in range), `std::optional` instead of sentinel values, `enum class`, no raw `new/delete`, input whitelist (only 4 valid direction vectors). |
| **Testable coding** | Pure functions (`CollisionDetector::check` is const and doesn't mutate the body), injectable RNG, controller `step()` callable without a clock or window. |
| **Coding standards** | `docs/CODING_STANDARDS.md`, `.clang-format`, `.editorconfig`. |
| **Sprint mgmt, code reviews, PRs** | One branch per member, 17 PRs, issue-linked commits ("closes #19"), PR template with a Definition of Done. |
| **Understanding an existing codebase** | Charan's later `main.cpp` integration on top of Tushar's; Ved's SAD v1.1 re-aligned to the real code. |
| **CI/CD, DevSecOps, build system** | CMake + GitHub Actions: build, tests, sanitizers, cppcheck, security scan, macOS; tag → release with packages, SBOM and checksums; Dependabot. |
| **Principles of testing / levels** | **Unit** (collision, food, score, movement) → **integration** (controller with real modules) → **system** (manual TC-01…TC-14 on the running game) → **acceptance** (demo). |
| **Static vs dynamic testing** | Static: cppcheck, compiler warnings, the security grep scan, document reviews. Dynamic: GoogleTest, sanitizers at runtime, manual gameplay. |
| **Performance testing** | FPS counter; three 60-second runs at ≈60 FPS against a target of ≥30. |
| **Release testing** | The release workflow refuses to publish unless all tests pass on both platforms. |
| **SCM, baselining, versioning** | Git, branches, merge commits, semantic versioning (v1.0.0), CHANGELOG, tag = baseline. |
| **Change control / impact analysis** | Changing the collision rule affects `Snake`, the controller and TC-05; the RTM shows which tests to re-run. |
| **Defect / bug management** | Bug-report template with severity; defects found: the tail-collision test setup (fixed by Ved), CI SFML incompatibility (fixed by Tushar), Mac font path (fixed by Ved), Release-mode asserts (fixed in v1.0.0). |
| **Release mgmt / Software BOM** | `release.yml`, SPDX SBOM, `docs/SBOM.md` (it flags the Arial licence risk). |

### Unit 4: Usability, quality, system testing
Quality metrics: 73 automated tests at a 100% pass rate · 29/29 requirements traced · 0 compiler warnings ·
0 cppcheck warnings · 0 sanitizer findings · ≈60 FPS. Reliability: 10 restart cycles. Usability: an
external reviewer understood the controls without help.

---

## Part 4 — Questions you can be asked (with answers)

### 1. Process and Agile
1. **Which SDLC model did you follow, and why?** Agile (Scrum-like). The requirements were small but likely
   to change, the team was part-time, and the course required sprints and demos. Short iterations gave a
   playable build early.
2. **What were your sprints?** Sprint 1, 19–23 Oct: the working game core, GitHub Actions, reviewed PRs and
   a 1-minute demo. Sprint 2, 26–30 Oct: the remaining features, testing, documentation and a 2-minute
   demo, then freeze.
3. **What is a product backlog vs a sprint backlog?** The product backlog is all prioritised work
   (issues #6–#22). The sprint backlog is the subset committed to one sprint (e.g. #16–#20 in Sprint 1).
4. **What is a user story? Give one.** "As a player I want the game to ignore a reverse key so that I don't
   die instantly." Acceptance: a direct or rapid double-key reversal is rejected (issue #16).
5. **Definition of Done?** Code merged via PR, tests added and passing, CI green, docs/RTM updated, reviewed
   by a teammate (`.github/pull_request_template.md`).
6. **What went wrong in your process?** A late start on code, self-merged PRs, one direct push to `main`,
   and demo videos not uploaded on time. Fixes: CODEOWNERS, the PR template, and recommended branch
   protection.
7. **Agile advantages and disadvantages?** For: early feedback, adapts to change, visible progress. Against:
   less upfront documentation and predictability, and it needs discipline. We saw that discipline problem
   with the reviews.

### 2. Requirements
8. **Functional vs non-functional, with an example of each?** FR-06: reject a 180° reversal. NFR-01: ≥30
   FPS average over 60 s.
9. **Why have security requirements in a game?** It is local software that reads input and files. SR-02 and
   SR-03 guarantee it can't be abused to run commands or touch user files, and SR-04 validates input
   against the state.
10. **How did you make NFRs measurable?** Concrete thresholds: 30 FPS over 60 s on the test PC, 10
    consecutive restart cycles, a fresh-clone build following the README.
11. **What is the RTM, and why is it useful?** A table linking each requirement to its tests and status.
    It shows that nothing is untested (coverage) and which tests to re-run when a requirement changes
    (impact analysis).
12. **Forward vs backward traceability?** Forward: requirement → test (does every requirement have a test?).
    Backward: test → requirement (does every test justify itself?). `RTM.md` has both.
13. **Explain your use-case diagram.** The Player starts, steers, eats, views the score, restarts and exits.
    Steer «includes» Validate direction and Detect collision; Detect collision is «extended» by Show
    Game Over when the collision is fatal. The Developer/Tester builds and runs the tests.
14. **How were requirement changes handled?** Through versioned documents (SRS v1.0 → v1.1 update, SAD
    v1.0 → v1.1). Increasing difficulty was deferred because it needed an SRS change first.

### 3. Architecture and design
15. **What architecture did you use?** Layered/modular in a single process: presentation (SFML) →
    application (GameController) → domain (Snake, FoodManager, ScoreManager, CollisionDetector).
16. **Why not MVC or microservices?** Microservices add a network and deployment for no benefit in a local
    game. The structure is close to MVC: the domain is the model, `main.cpp` rendering is the view, and
    GameController plus input handling is the controller.
17. **Which design patterns are present?** State machine (GameState), controller/mediator (GameController),
    dependency injection (the RNG), value objects (Cell), facade (the controller's API to the UI).
18. **Explain the GameController state machine.** Ready —Enter/Space→ Playing —fatal collision→ GameOver
    —R→ Playing. Playing —board full→ RoundComplete —R→ Playing. Input and update are ignored outside
    Playing.
19. **Walk through one game tick (`step()`).**
    1. Compute the next head without moving.
    2. `willGrow = food.isAt(nextHead)`.
    3. `collision.check(nextHead, body, bounds, willGrow)`.
    4. If it is a collision → GameOver, keeping the score.
    5. Otherwise `snake.advance(willGrow)`.
    6. If it grew → +10 and place new food (BoardFull → RoundComplete).
20. **Why check the collision *before* moving?** The state is never invalid (no snake drawn inside a
    wall), Game Over shows the last valid position, and the check is a pure function of the proposed move.
21. **What is the departing-tail rule?** On a normal move the tail leaves its cell in the same tick, so the
    head may enter that cell safely. When growing, the tail stays and that cell is a collision. The code
    checks `body.size() - (grows ? 0 : 1)` segments.
22. **How is game speed independent of frame rate?** Fixed timestep: `update(dt)` accumulates time and
    runs `step()` every 0.15 s, while rendering runs at up to 60 FPS.
23. **How do you stop a "double-key reversal"?** For example, moving right, the player presses Up then
    Left within one tick. An input latch (`directionChangedThisStep`) accepts only one direction change per
    tick.
24. **How is food placed? Why not random-retry?** Enumerate all free cells and pick one uniformly with
    `mt19937`. This is O(cells), never loops forever, and detects BoardFull immediately. Random-retry gets
    slower as the snake fills the board and never ends on a full board.
25. **Coupling and cohesion in your design?** High cohesion: each class does one job. Low coupling: the
    domain classes only share `grid_types.h`; only the controller knows all of them.
26. **What would you change in the design?** Move SFML out of `Snake` (store `Cell`s only), extract a
    `Renderer` from `main.cpp`, and put `Snake` in the `snakegame` namespace (see the tech-debt register).
27. **How is error handling designed?** Invalid configuration fails loudly (an exception, or an
    `InvalidBounds` result). Expected end conditions are states, not errors (BoardFull → RoundComplete).
    A missing font falls back to system fonts.

### 4. Implementation and coding
28. **Why C++ and SFML?** Performance, deterministic control, a course-friendly language; SFML is a light
    2D library for window, input, shapes and fonts.
29. **What makes the code testable?** Pure domain logic with no SFML in the collision, food and score
    modules; RNG injection; `step()` can be called directly without a clock or window.
30. **Give examples of defensive coding.** Bounds checks before array indexing, validating direction
    vectors against an allowed set, `std::optional` for food, `enum class`, `[[nodiscard]]`, exceptions on
    invalid bounds.
31. **Why `enum class` rather than `enum`?** It is scoped and has no implicit int conversion, which
    prevents mixing up `GameState` and `CollisionType` values.
32. **What coding standard do you follow?** `docs/CODING_STANDARDS.md`: C++17, zero warnings, naming
    conventions, `.clang-format` (4 spaces, Allman braces, 100 columns).
33. **What is `[[nodiscard]]` for?** The compiler warns if a caller ignores a result that matters (e.g. a
    collision result).

### 5. Testing
34. **How many tests, of which types?** 73 CTest tests in 5 targets:
    - collision: 23 unit tests
    - food/score: 19 unit tests
    - food/score integration: 10
    - game integration: 20
    - snake movement: 1 CTest entry running 6 checks
    Plus 14 manual system test cases (TC-01…TC-14).
35. **Unit vs integration vs system vs acceptance testing, with examples?**
    - Unit: `CollisionDetector` alone.
    - Integration: `GameController` with real Snake, Food and Score objects.
    - System: playing the built game against TC-01…TC-14.
    - Acceptance: the demo to the instructor.
36. **What is a parameterised test?** One test body run with many inputs, e.g. `WallCollisionTest` runs
    for each wall position (`TEST_P` with `INSTANTIATE_TEST_SUITE_P`).
37. **How do you test random food placement?** Inject a seeded `mt19937`, check 500 seeds for "inside the
    board and never on the snake", the same seed giving the same cell, the only free cell being chosen,
    and every free cell being reachable.
38. **Black box vs white box?** The TC-xx system tests are black box (behaviour only). The departing-tail
    tests are white box (they target a specific branch in `check`).
39. **Static vs dynamic testing in your project?** Static: cppcheck, `-Werror`, the security grep, document
    reviews. Dynamic: GoogleTest, ASan/UBSan at runtime, manual play.
40. **What are sanitizers?** Compiler instrumentation that detects memory errors and undefined behaviour
    while the tests run. AddressSanitizer catches out-of-bounds access, use-after-free and leaks;
    UndefinedBehaviorSanitizer catches signed overflow, invalid shifts and similar. CI runs every test
    under both.
41. **What is regression testing here?** All 73 tests run on every push. E.g. scenario 12 guards the
    departing-tail rule against regressions.
42. **Describe a bug your tests found.**
    - The TC-05 self-collision setup in the original Test Plan could hit the *departing tail*, so it would
      not reliably collide. It was redesigned to hit a non-tail segment (Ved's "fix tail test setup",
      `d76af8b`).
    - The movement tests used `assert`, which Release builds remove, so they passed without checking.
      Found and fixed in v1.0.0.
43. **What is the test oracle for TC-08 (performance)?** Average FPS ≥30 over a 60-second run, read from
    the in-game FPS counter. Three runs gave ≈60.
44. **What's in a test plan (IEEE 829)?** Test items, features to test and not to test, approach,
    pass/fail criteria, suspension criteria, environment, schedule, responsibilities, risks, test cases,
    and the RTM.

### 6. SCM, Git and code review
45. **What is your branching strategy?** One personal branch per member → PR into `main`; pull `main`
    before pushing.
46. **What is a pull request, and why use one?** A request to merge a branch, with a diff, discussion, CI
    results and review. It is the quality gate before `main`.
47. **Merge vs rebase?** A merge keeps history with a merge commit (what we used, e.g. "Merge pull request
    #29"). A rebase rewrites commits onto the new base for a linear history.
48. **What is a baseline?** An approved, frozen version used as a reference: the SRS v1.0 baseline (12 Oct)
    and the release tag v1.0.0.
49. **Semantic versioning?** MAJOR.MINOR.PATCH, incremented for breaking change, new feature, and bug fix
    respectively.
50. **Did you do code reviews?** Partly. Ved's PRs were merged by other members, and Charan reviewed the
    test cases on issue #9. Many PRs were self-merged; we now require a non-author review through
    CODEOWNERS and the PR template. (Be honest; the examiner can see it.)
51. **How do you handle merge conflicts?** Pull `main` into the branch, resolve locally, rebuild, re-run
    the tests, push. Owning separate files per module kept conflicts rare.

### 7. CI/CD and DevOps
52. **What is CI? What is CD?** CI: every change is built and tested automatically. Continuous
    *Delivery*: every passing change can be released at the push of a button or tag. Continuous
    *Deployment*: it goes live automatically.
53. **What does your CI run?** Five parallel jobs:
    1. Linux build with `-Werror` and all tests (with a JUnit report)
    2. ASan + UBSan
    3. cppcheck and the clang-format report
    4. the security scan
    5. macOS build and test
54. **What does your CD do?** On a `v*.*.*` tag it builds and tests on Linux and macOS, packages with
    CPack, generates an SPDX SBOM and SHA-256 checksums, and creates a GitHub Release using the CHANGELOG.
55. **Why the `xvfb-run`?** It provides a virtual X display on the headless Linux runner, in case any
    SFML-linked code needs one.
56. **Why pin `ubuntu-22.04` and GoogleTest's commit?** Reproducible builds. A moving `ubuntu-latest` or
    `main` dependency can break the build without any code change. Tushar's fix in PR #28 was exactly this.
57. **What is DevSecOps here?** Security checks inside the pipeline: sanitizers, static analysis, the
    forbidden-API scan, Dependabot updates, a least-privilege `GITHUB_TOKEN`, the SBOM and checksums.
58. **What is an SBOM, and why does it matter?** A Software Bill of Materials lists every component and
    version (SFML, GoogleTest, the font). It enables vulnerability and licence checks. Ours flagged that
    `arial.ttf` is proprietary.
59. **Why `-Werror`?** It turns warnings into failures so quality can't silently degrade.
60. **What is a build system? Why CMake?** It turns source into binaries reproducibly. CMake is
    cross-platform (generates Make, Ninja or Visual Studio files), and gives us FetchContent for
    GoogleTest, CTest and CPack.
61. **How would you do a release right now?** `git tag v1.0.0 && git push origin v1.0.0`, then watch
    Actions → Release.

### 8. Quality, metrics and maintenance
62. **Which quality metrics can you quote?** 100% pass rate across 73 tests, 29/29 requirements traced,
    0 warnings, 0 cppcheck warnings, 0 sanitizer findings, ≈60 FPS, 10/10 restart cycles.
63. **What is technical debt? Give yours.** The cost of shortcuts that will need rework later: SFML types
    inside `Snake`, a 430-line `main.cpp`, a proprietary font, the hand-rolled movement test harness.
64. **Verification vs validation?** Verification: did we build it right (tests against the SRS)?
    Validation: did we build the right thing (the demo and the usability check with an external user)?
65. **How would you add "increasing difficulty"?**
    1. Raise a change request and update the SRS (a new FR).
    2. Impact analysis: `GameController::movementInterval`, the HUD, and new tests.
    3. Decrease the interval every N foods, with a floor.
    4. Add unit and integration tests, and update the RTM.

### 9. AI in SE
66. **Did you use AI? How do you ensure quality?** Yes, for drafting documents and reviewing consistency.
    Every AI output was checked by a teammate against the code and the other documents. The AI drafts had
    wrong SRNs and responsibilities that we corrected. Tests and CI verify the code regardless of who or
    what wrote it.
67. **What are the risks of vibe coding?** Plausible but wrong code or claims, licence issues, and
    security holes. Mitigations: code review, tests, static analysis and humans owning every merge.

### 10. Person-specific questions (expect these)
- **Charan:** How does `changeDirection` reject reversals and double turns? Why does `Snake` use pixel
  vectors (±20) for direction? (It's tied to SFML rendering; that's tech debt.) How did you measure FPS?
  What was in the SRS security section? How did you execute TC-01…TC-14?
- **Vrunda:** How is the best score kept across restarts but the score reset? (`resetRound()` zeroes the
  score and food count, not `bestScore`.) How do you prove food never spawns on the snake? Explain the
  IEEE 829 sections of your test plan. What is the severity table? Why did TC-05 need a controlled layout?
- **Ved:** Explain the departing-tail rule and its tests. Why make `CollisionDetector` pure? Explain both
  sequence diagrams. Why a layered architecture? What does the SAD security architecture say?
- **Tushar:** Draw the state machine. Why does `step()` check before moving? Explain `ci.yml` line by line.
  Why Ubuntu 22.04? How does the RTM connect the SRS and the Test Plan? How is restart made atomic?

---

## Part 5 — Demo checklist for 17 Nov
1. `cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure` → 73/73.
2. Launch the game: Ready screen → Enter → steer → reversal ignored → eat 3 food (score 30, length 6) →
   wall Game Over (reason + final score + best) → R → self-collision → Esc.
3. Show GitHub: the backlog/Projects board, PRs, green Actions run (5 jobs), `docs/` folder, RTM.
4. Optionally push tag `v1.0.0` and show the Release (packages + SBOM).
5. **Still to do before the review:** upload the demo videos (issues #21, #22); close #7, #11, #12 with
   a comment linking evidence; consider enabling branch protection on `main`.

## Part 6 — Honest weak spots (prepare an answer, don't hide them)

| Weak spot | Good answer |
|---|---|
| Most PRs merged by their author with no formal GitHub review | "We reviewed in chat and in issue comments, but it isn't recorded on the PRs. We've added CODEOWNERS and a PR checklist, and branch protection is the next step." |
| Vrunda's scoring commits went straight to `main` | "It was the night before the review. The code is covered by 29 tests that run in CI." |
| Code started late (23 Oct) compared with the sprint plan | "Phase 1 documents took longer; we compressed implementation into Sprint 1 and verified it in Sprint 2." |
| Demo videos not uploaded | Upload them before the presentation. |
| TC-14 security trace was only partially manual | "We automated it as a static forbidden-API scan in CI. A runtime file-activity trace is optional." |
| Proprietary Arial font | "Our SBOM flagged it; the fix is to swap in DejaVu Sans." |
