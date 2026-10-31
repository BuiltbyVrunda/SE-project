# Food Generation & Scoring Validation Report

**Module owner:** Vrunda C (PES1UG24CS541)
**Module:** Food Generation & Score (`src/food_manager.*`, `src/score_manager.*`)
**Requirements:** FR-07 to FR-11
**Test Plan cases:** TC-03 (food placement, detection and snake growth), TC-04 (score update and replacement food)
**Date:** 31 October 2026

---

## 1. What the module does

| Feature | Where | Requirement |
| :--- | :--- | :--- |
| Food is placed at random on a cell that is inside the board and not on the snake | `FoodManager::place` | FR-07 |
| Food is detected when the snake's next head cell is the food cell | `FoodManager::isAt`, `GameController::step` | FR-08 |
| The snake grows by exactly one segment per food (tail stays in place) | `Snake::advance(true)` called from `GameController::step` | FR-09 |
| Each food adds 10 points to the score | `ScoreManager::addFoodPoints` | FR-10 |
| A replacement food appears right after eating; only one food exists at a time | `FoodManager::place` after each pickup | FR-11 |
| Full board is handled without an infinite loop (round ends as a win) | `PlacementResult::BoardFull` | FR-07 |
| Best score of the session is kept across restarts and shown on screen | `ScoreManager::getBestScore`, `main.cpp` HUD | FR-10 (extension) |

Placement picks from a list of free cells instead of retrying random cells, so it always finishes and never lands on the snake. Placement accepts a seeded `std::mt19937`, which makes the tests repeatable.

---

## 2. Automated tests

### 2.1 `food_score_tests` (unit tests, no window needed)

| Test | Checks | TC | Result |
| :--- | :--- | :---: | :---: |
| NoFoodExistsBeforeFirstPlacement | No food before placement | TC-03 | PASS |
| FoodIsAlwaysInsideBoardAndOffSnakeForManySeeds | 500 seeds: food inside board, never on snake | TC-03 | PASS |
| SameSeedGivesSamePositionForRepeatableTests | Same seed gives same cell | TC-03 | PASS |
| OnlyRemainingFreeCellIsChosen | 3x3 board with one free cell: that cell is chosen every time | TC-03 | PASS |
| EveryFreeCellCanBeChosen | All free cells are reachable (placement is really random) | TC-03 | PASS |
| BoardFullClearsFoodAndReportsIt | Full board returns `BoardFull`, no food left | TC-03 | PASS |
| InvalidBoardClearsFoodAndReportsIt | Zero/negative board size returns `InvalidBounds` | TC-03 | PASS |
| NewPlacementReplacesOldFood | Only one food at a time; old cell is cleared | TC-04 | PASS |
| IsAtMatchesOnlyTheFoodCell | Detection matches only the exact food cell | TC-03 | PASS |
| ResetRemovesFood | Reset clears food | TC-03 | PASS |
| StartsAtZero | Score, best score and food count start at 0; value is 10 | TC-04 | PASS |
| EachFoodAddsExactlyTenPoints | +10 per food | TC-04 | PASS |
| ScoreEqualsFoodCountTimesFoodValue | 5 foods = 50 points | TC-04 | PASS |
| BestScoreFollowsHighestScore | Best score tracks the highest score | TC-04 | PASS |
| ResetRoundClearsScoreButKeepsBest | New round: score 0, best kept | TC-06 | PASS |
| LowerLaterRoundDoesNotLowerBest | A lower later round does not lower the best | TC-04 | PASS |
| HigherLaterRoundRaisesBest | A higher later round raises the best | TC-04 | PASS |
| CustomFoodValueIsUsed | Configurable points per food | TC-04 | PASS |
| NonPositiveFoodValueFallsBackToDefault | 0 or negative value falls back to 10 | TC-04 | PASS |

**Result: 19/19 passed.**

### 2.2 `food_score_integration_tests` (through `GameController`)

| Test | Checks | TC | Result |
| :--- | :--- | :---: | :---: |
| InitialFoodIsInsideBoardAndOffSnake | Food at round start is valid; score starts at 0 | TC-01 | PASS |
| EatingFoodGrowsSnakeByOneAndKeepsTail | Body after eating is exactly head + old body | TC-03 | PASS |
| FoodNextToPathIsNotEaten | Food diagonal to the head is not eaten | TC-03 | PASS |
| OneConsumptionEventPerFood | Eating counts once, not again on later moves | TC-03 | PASS |
| FiveConsecutiveFoodsGiveFiftyPoints | 5 foods: score 50, length 8, valid replacement each time | TC-04 | PASS |
| ReplacementFoodNeverLandsOnLongSnake | 30 foods: replacement never on a 33-segment snake | TC-04 | PASS |
| FinalScoreIsKeptAtGameOver | Final score equals score at collision | TC-05 | PASS |
| RestartResetsScoreButKeepsBestScore | Restart: score 0, best kept, lower round keeps best | TC-06 | PASS |
| RestartPlacesNewFoodOffResetSnake | New food after restart is valid | TC-06 | PASS |
| NoScoreChangeAfterGameOver | No score change after game over | TC-05 | PASS |

**Result: 10/10 passed.**

### 2.3 Full suite

| Build | Tests | Result |
| :--- | :---: | :---: |
| Full build (`BUILD_GAME=ON`) | 68 | 68/68 PASS |
| Headless build (`BUILD_GAME=OFF`) | 42 | 42/42 PASS |
| Full build with AddressSanitizer + UndefinedBehaviorSanitizer | 68 | 68/68 PASS, no sanitizer errors |

Earlier tests from the other modules (collision 23, movement 6, integration 10) were re-run and all still pass, so moving the score into `ScoreManager` did not change any existing behaviour.

---

## 3. Screen checks

Run on a virtual display (Xvfb) with the built `SnakeGame`.

| Check | Expected | Result |
| :--- | :--- | :---: |
| HUD while playing | "Score: X" top-left, "Best: X" top-right | PASS |
| Game Over screen | "Final Score: X" with "Best Score: X" below it, restart and exit lines still fit in the box | PASS |
| Restart with R | New round starts, score back to 0 | PASS |

Still to check by hand on the team's Windows machine before the demo: eat a few foods, die, restart, and confirm "Best" keeps the old score while "Score" starts again from 0.

---

## 4. Environment

* Language: C++17
* Build system: CMake 3.28
* Compiler: GCC 13.3 (Ubuntu 24.04)
* Graphics library: SFML 2.6.1
* Test framework: GoogleTest 1.17.0 with CTest

---

## 5. Conclusion

Food placement, food detection, growth, scoring and replacement food meet FR-07 to FR-11. All 29 food and scoring tests pass, the full 68-test suite passes, and the sanitizer run found no memory or undefined-behaviour errors.
