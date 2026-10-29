# Snake Game

A software engineering project developed as part of the Software Engineering course at PES University.

## Team Members & Responsibilities

| Name | SRN | Primary Responsibilities | Modules |
| :--- | :--- | :--- | :--- |
| **Yele Charan** | PES1UG25CS856 | SRS, snake movement logic, direction validation | `src/snake.h`, `src/snake.cpp`, `tests/snake_test.cpp` |
| **Vrunda C** | PES1UG24CS541 | Test plan, food generation, growth, scoring | `src/food_manager.h`, `src/food_manager.cpp` |
| **Ved Mudkavi** | PES1UG24CS522 | Architecture & design, collision detection | `src/collision_detector.h`, `src/collision_detector.cpp`, `tests/collision_detector_test.cpp` |
| **Tushar N** | PES1UG24CS504 | Integration, lifecycle, Game Controller, UI, CI/CD | `src/game_controller.h`, `src/game_controller.cpp`, `src/main.cpp`, `tests/game_integration_test.cpp`, `.github/workflows/ci.yml` |

---

## Architecture and Module Integration

The application follows a modular architecture separating presentation, domain rules, and lifecycle coordination.

```
+-----------------------------------------------------------+
|                      Presentation                         |
|  SFML RenderWindow, Event Polling, HUD, Start/GameOver UI |
+-----------------------------+-----------------------------+
                              | (user input / render calls)
                              v
+-----------------------------------------------------------+
|                     Game Controller                       |
|   State Machine: Ready -> Playing -> GameOver / Complete  |
|      Coordinates pre-movement collision check & reset     |
+-------------+---------------+-----------------------------+
              |               |
              v               v
+-----------------------+   +-------------------------------+
|         Snake         |   |         Food Manager          |
|  Body segments (grid) |   |  Random unoccupied placement  |
|  Direction & latch    |   |  Consumption & score tracking |
+-------------+---------+   +---------------+---------------+
              |                             |
              +--------------+--------------+
                             v
              +-----------------------------+
              |     Collision Detector      |
              |  Pure Boundary & Self check |
              |  TC-05 Departing tail rule  |
              +-----------------------------+
```

### 1. Snake Movement (`src/snake.h`, `src/snake.cpp`)
- Tracks snake segments in both pixel space (for rendering) and discrete grid coordinates.
- Manages discrete movement direction (Up, Down, Left, Right).
- Enforces direction validation: rejects immediate 180-degree reversals and rapid multi-key turn reversals within the same movement tick using an input latch.
- Supports atomic advancement and growth (`advance(bool growSnake)`).

### 2. Food Placement & Scoring (`src/food_manager.h`, `src/food_manager.cpp`)
- Places food randomly strictly on unoccupied grid cells (`place(bounds, body, rng)`).
- Eliminates infinite random retry loops by identifying free cells directly.
- Detects board saturation (`PlacementResult::BoardFull`).
- Awards 10 points per food item and triggers snake growth by 1 segment.

### 3. Collision Detection (`src/collision_detector.h`, `src/collision_detector.cpp`)
- Pure domain logic with zero SFML dependencies.
- Validates the candidate head position *before* movement is committed.
- Detects boundary collisions against all four walls (`0 <= x < columns`, `0 <= y < rows`).
- Detects self-collisions against occupied snake segments.
- Enforces the **departing tail rule** (SAD TC-05): on a non-growing move, the tail segment leaves its cell in the same tick, making that cell safe to enter; on a growing move, the tail stays and is treated as occupied.

### 4. Game Controller & Lifecycle (`src/game_controller.h`, `src/game_controller.cpp`)
- State machine managing `Ready`, `Playing`, `GameOver`, and `RoundComplete`.
- Coordinates the tick sequence:
  1. Poll player input and latch direction change.
  2. Compute candidate next head position without modifying the snake.
  3. Query food presence at candidate head (`willGrow`).
  4. Perform collision check with candidate head and `willGrow` flag.
  5. If fatal collision occurs (`Boundary` or `Self`), immediately enter `GameOver`, leaving snake and score intact.
  6. If safe (`None`), commit snake advancement (`advance(willGrow)`), increment score by 10, and request replacement food.
- Handles atomic restart: resets snake to initial 3 segments at `(5, 5)`, direction to Right, score to 0, regenerates food on a free cell, and clears game-over state.

---

## Keyboard Controls

| Key | Context | Action |
| :--- | :--- | :--- |
| **Enter / Space** | Ready screen | Start new round |
| **Arrow Up** | Playing | Move snake Up |
| **Arrow Down** | Playing | Move snake Down |
| **Arrow Left** | Playing | Move snake Left |
| **Arrow Right** | Playing | Move snake Right |
| **R / r** | Game Over / Win | Restart game (clean reset) |
| **Escape** | Any | Exit game cleanly |

---

## Build Instructions

### Prerequisites
- C++17 compliant compiler (GCC 11+, Clang 13+, or MSVC 2019+)
- CMake 3.20 or newer
- SFML 2.6.x (installed via package manager or configured via `SFML_DIR`)
- Git (for automatic GoogleTest FetchContent)

### Linux (Ubuntu/Debian)
```sh
# Install dependencies
sudo apt-get update
sudo apt-get install -y build-essential cmake libsfml-dev

# Configure and build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_GAME=ON -DBUILD_TESTING=ON
cmake --build build --config Release

# Run automated tests
ctest --test-dir build --output-on-failure -C Release

# Run the game
./build/SnakeGame
```

### Windows (GCC / MinGW or MSVC)
```sh
# Configure specifying SFML path if not installed globally
cmake -S . -B build -DBUILD_GAME=ON -DCMAKE_BUILD_TYPE=Release -DSFML_DIR="<path_to_SFMLConfig.cmake>"
cmake --build build --config Release

# Run automated tests
ctest --test-dir build --output-on-failure -C Release

# Launch the game
build\SnakeGame.exe
```

---

## Automated Test Suites

The repository contains automated tests across three targets, covering all functional requirements:

| Test Target | Framework | Description | Coverage |
| :--- | :--- | :--- | :--- |
| `collision_tests` | Google Test | Pure collision detector verification | 23 tests covering wall collisions, valid cells, tail rules, TC-05 fixtures, invalid bounds |
| `SnakeTests` | Standalone | Charan's snake movement tests | 6 tests covering initial direction, turns, reversal rejection, forward step |
| `game_integration_tests` | Google Test | Integrated game controller and food logic | 10 tests covering all 12 required scenarios, food manager logic, and restart lifecycle |

To run all 39 tests:
```sh
ctest --test-dir build --output-on-failure --verbose
```

### Test Scenarios Covered
1. **Normal snake movement:** snake advances 1 cell in current heading per tick.
2. **Permitted direction steering:** snake successfully turns Up, Down, Left, and Right.
3. **Invalid reversal prevention:** immediate opposite directions and double-turn reversals are rejected.
4. **Wall collision:** head hitting board boundary stops gameplay and triggers `GameOver`.
5. **Self-collision:** snake looping back into its own body triggers `GameOver`.
6. **Food consumption & scoring:** eating food increases score by 10, increases length by 1, and spawns new food.
7. **Game Over behavior:** fatal collision halts movement, preserves final score, and presents Game Over screen.
8. **Movement stoppage after Game Over:** subsequent movement ticks or input do not alter snake coordinates or score.
9. **Restart after Game Over:** pressing restart restores playable state.
10. **State reset:** position resets to initial 3 segments, direction resets to Right, score resets to 0, food is placed on a free cell.
11. **Continued gameplay after restart:** subsequent steering, food collection, and scoring work seamlessly.
12. **Collision regression testing:** departing tail rule (TC-05) verifies tail vacates safely on non-growing moves.

---

## Continuous Integration (GitHub Actions)

A GitHub Actions workflow is configured under `.github/workflows/ci.yml`.
- **Triggers:** Pushes and pull requests on all team branches (`main`, `charan`, `tushar`, `ved`, `vrunda`).
- **Platform:** Ubuntu with headless virtual display (`xvfb`).
- **Steps:** Checks out repository, installs `libsfml-dev` and build tools, configures CMake, builds all targets, and runs the complete test suite with CTest.

---

## Known Limitations

1. **Local Desktop Application:** The game is designed for local single-player execution; network multiplayer and online leaderboards are out of scope.
2. **SFML Version:** Developed and tested against SFML 2.6.x. SFML 3.x is not supported due to breaking API changes.
3. **Display Requirement for Game Launch:** Playing the interactive graphical game requires a desktop display environment; automated logic tests run headlessly in CI without graphical display dependencies.
