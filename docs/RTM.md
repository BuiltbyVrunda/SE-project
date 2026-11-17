# Requirements Traceability Matrix (RTM) — Snake Game

Forward traceability: **SRS requirement → STP test case → automated test / CI job → status**. 
Original team workbook: `docs/RTM_SnakeGame.xlsx` (Tushar N). This Markdown/CSV version adds the automated-evidence and owner columns and reflects the CI quality gates added in v1.0.0. See the [system validation report](system_validation_report.md) for the current TC-14 evidence status.

| ID | Requirement | Category | Test case | Automated evidence | Owner | Status |
|---|---|---|---|---|---|---|
| FR-01 | The system shall start a new round when the player invokes the agreed start action. | Functional | TC-01 | tests/game_integration_test.cpp: Scenario09/11 (startRound, Ready->Playing) | Tushar N | Pass |
| FR-02 | The system shall display a bounded play area. | Functional | TC-01 | tests/game_integration_test.cpp: WallIntegrationTest (40x30 bounded grid) | Tushar N | Pass |
| FR-03 | The system shall place the initial snake fully inside the play area. | Functional | TC-01 | tests/game_integration_test.cpp: Scenario10 (reset to 3 segments at (5,5)) | Yele Charan | Pass |
| FR-04 | The system shall accept documented keyboard keys to change direction. | Functional | TC-02 | tests/snake_test.cpp; tests/game_integration_test.cpp: Scenario02 | Yele Charan | Pass |
| FR-05 | The system shall move the snake in its current direction during gameplay. | Functional | TC-02 | tests/snake_test.cpp; tests/game_integration_test.cpp: Scenario01 | Yele Charan | Pass |
| FR-06 | The system shall reject an immediate 180-degree direction reversal. | Functional | TC-02, TC-13 | tests/snake_test.cpp; tests/game_integration_test.cpp: Scenario03 | Yele Charan | Pass |
| FR-07 | The system shall place food at a valid unoccupied position inside the play area. | Functional | TC-03 | tests/food_score_test.cpp: FoodPlacementTest (500 seeds, last free cell, BoardFull) | Vrunda C | Pass |
| FR-08 | The system shall detect when the snake reaches food. | Functional | TC-03 | tests/food_score_test.cpp: FoodDetectionTest; tests/food_score_integration_test.cpp | Vrunda C | Pass |
| FR-09 | The system shall increase snake length after food is consumed. | Functional | TC-03 | tests/food_score_integration_test.cpp: growth by one; tests/game_integration_test.cpp: Scenario06 | Vrunda C | Pass |
| FR-10 | The system shall increase score by the configured value when food is consumed. | Functional | TC-04 | tests/food_score_test.cpp: ScoreManagerTest; tests/food_score_integration_test.cpp: 5-food score | Vrunda C | Pass |
| FR-11 | The system shall generate replacement food after food is consumed. | Functional | TC-04 | tests/food_score_integration_test.cpp: replacement food; tests/food_score_test.cpp: NewPlacementReplacesOldFood | Vrunda C | Pass |
| FR-12 | The system shall detect collision between snake head and boundary. | Functional | TC-05 | tests/collision_detector_test.cpp: WallCollisionTest; tests/game_integration_test.cpp: Scenario04 | Ved Mudkavi | Pass |
| FR-13 | The system shall detect collision between snake head and its own body. | Functional | TC-05 | tests/collision_detector_test.cpp: CollisionDetectorTest; tests/game_integration_test.cpp: Scenario05/12 | Ved Mudkavi | Pass |
| FR-14 | The system shall stop snake movement after a fatal collision. | Functional | TC-05 | tests/game_integration_test.cpp: Scenario07/08 | Tushar N | Pass |
| FR-15 | The system shall display a game-over message and final score after a fatal collision. | Functional | TC-05 | tests/game_integration_test.cpp: Scenario07, FatalCollisionKeepsEarnedScore... | Tushar N | Pass |
| FR-16 | The system shall allow restart after game over. | Functional | TC-06 | tests/game_integration_test.cpp: Scenario09 | Tushar N | Pass |
| FR-17 | The system shall reset snake, direction and score when a new round starts. | Functional | TC-06 | tests/game_integration_test.cpp: Scenario10; tests/food_score_integration_test.cpp: best score across restart | Tushar N | Pass |
| FR-18 | The system shall close safely when the player closes the game window. | Functional | TC-07 | CI sanitizers job (ASan/UBSan, leak check) + manual TC-07 | Tushar N | Pass |
| NFR-01 | Average at least 30 FPS during a 60-second normal gameplay run on the agreed test computer. | Performance | TC-08 | Manual TC-08 (in-game FPS counter, src/main.cpp) | Yele Charan | Pass |
| NFR-02 | Controls and restart action are described in the interface or README. | Usability | TC-09 | Manual TC-09; README "Keyboard Controls" | Tushar N | Pass |
| NFR-03 | Complete 10 consecutive start/restart cycles without crashing. | Reliability | TC-10 | tests/game_integration_test.cpp: Scenario09-11 restart cycles; manual TC-10 | Tushar N | Pass |
| NFR-04 | Movement, food, collision, game control and rendering responsibilities are identifiable in modules/classes where practical. | Maintainability | TC-11 | CMake module libraries (snake, snake_food, snake_score, snake_collision, snake_controller) | Ved Mudkavi | Pass |
| NFR-05 | Build and launch with the compiler and SFML version documented by the team. | Compatibility | TC-12 | CI build-and-test (Ubuntu) + macos-build jobs | Tushar N | Pass |
| NFR-06 | README lists required tools, SFML dependency and build/run steps. | Documentation | TC-12 | CI builds from README commands on a clean runner | Tushar N | Pass |
| SR-01 | Ignore unsupported keyboard keys without crashing. | Security | TC-13 | tests/game_integration_test.cpp: input ignored when not Playing; manual TC-13 | Yele Charan | Pass |
| SR-02 | Do not execute operating-system commands in response to player input. | Security | TC-14 | CI security-scan job (scripts/security_scan.sh) | Ved Mudkavi | Static scan passed. Runtime trace pending. |
| SR-03 | Do not read or modify unrelated user files during normal gameplay. | Security | TC-14 | CI security-scan job (scripts/security_scan.sh) | Ved Mudkavi | Static scan passed. Runtime trace pending. |
| SR-04 | Validate direction changes against the current game state. | Security | TC-13 | tests/snake_test.cpp; tests/game_integration_test.cpp: Scenario03 | Yele Charan | Pass |
| SR-05 | Handle window-close events and shutdown safely. | Security | TC-07 | CI sanitizers job + manual TC-07 | Tushar N | Pass |

## Coverage summary

| Group | IDs | Count | Test cases | Status |
|---|---|---|---|---|
| Functional | FR-01 – FR-18 | 18 | TC-01 – TC-07 | All Pass |
| Non-functional | NFR-01 – NFR-06 | 6 | TC-08 – TC-12 | All Pass |
| Security | SR-01 – SR-05 | 5 | TC-07, TC-13, TC-14 | 3 Pass, 2 pending runtime evidence |
| **Total** | | **29** | TC-01 – TC-14 | Pass: 27. Runtime evidence pending: 2 |

## Verification notes

- **FR-01** — TC-01 manual startup/display check reported Pass. Automated suite: 73/73 passed.
- **FR-02** — TC-01 manual bounded play-area/display check reported Pass.
- **FR-03** — TC-01 manual initial snake placement check reported Pass.
- **FR-04** — TC-02/TC-13 input and direction checks reported Pass.
- **FR-05** — TC-02 movement checks reported Pass; automated movement/integration tests passed.
- **FR-06** — TC-02/TC-13 reversal and rapid-input checks reported Pass; automated movement tests passed.
- **FR-07** — TC-03 food placement checks reported Pass; automated food-placement tests passed.
- **FR-08** — TC-03 food detection checks reported Pass; automated food-detection/integration tests passed.
- **FR-09** — TC-03 snake growth checks reported Pass; automated growth tests passed.
- **FR-10** — TC-04 score update checks reported Pass; automated ScoreManager tests passed.
- **FR-11** — TC-04 replacement-food checks reported Pass; automated food-placement tests passed.
- **FR-12** — TC-05 collision checks reported Pass; automated collision tests passed.
- **FR-13** — TC-05 self-collision checks reported Pass; automated collision tests passed.
- **FR-14** — TC-05 fatal-collision/game-over checks reported Pass; automated integration tests passed.
- **FR-15** — TC-05 game-over/final-score checks reported Pass; automated integration tests passed.
- **FR-16** — TC-06 restart checks reported Pass; automated restart integration tests passed.
- **FR-17** — TC-06 reset-state checks reported Pass; automated score/reset integration tests passed.
- **FR-18** — TC-07 manual safe-close check reported Pass; sanitizer sub-step now automated in CI (ASan/UBSan + leak detection over the full suite).
- **NFR-01** — TC-08: three 60-second runs reported approximately 60 FPS each, exceeding the 30 FPS target.
- **NFR-02** — TC-09: external reviewer reportedly understood controls without assistance.
- **NFR-03** — TC-10: ten consecutive start/game-over/restart cycles reportedly completed without crash or freeze.
- **NFR-04** — TC-11: module responsibilities reviewed and reported identifiable in source.
- **NFR-05** — TC-12: teammate completed a fresh clone/build/launch successfully. Local CTest: 73/73 passed.
- **NFR-06** — TC-12: teammate fresh-clone walkthrough reportedly succeeded; confirm README instructions remain current.
- **SR-01** — TC-13 unsupported-key and invalid-state input checks reported Pass.
- **SR-02** — TC-14: automated forbidden-API scan (system/popen/exec/fork/CreateProcess) runs on every push and is clean. The Test Plan also requires a runtime file activity trace. That evidence is pending.
- **SR-03** — TC-14: the CI source scan passed. File activity during a full gameplay round, restart and close has not been verified with the requested runtime trace.
- **SR-04** — TC-13 direction validation and reversal checks reported Pass.
- **SR-05** — TC-07 manual window-close check reported Pass; sanitizer sub-step now automated in CI.

## Backward traceability (test → requirement)

| Test case | Requirements |
|---|---|
| TC-01 | FR-01, FR-02, FR-03 |
| TC-02 | FR-04, FR-05, FR-06 |
| TC-03 | FR-07, FR-08, FR-09 |
| TC-04 | FR-10, FR-11 |
| TC-05 | FR-12, FR-13, FR-14, FR-15 |
| TC-06 | FR-16, FR-17 |
| TC-07 | FR-18, SR-05 |
| TC-08 | NFR-01 |
| TC-09 | NFR-02 |
| TC-10 | NFR-03 |
| TC-11 | NFR-04 |
| TC-12 | NFR-05, NFR-06 |
| TC-13 | FR-06, SR-01, SR-04 |
| TC-14 | SR-02, SR-03 |
