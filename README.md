# Snake Game

A software engineering mini-project developed as part of the Software Engineering course at PES University.

## Project Overview

Snake Game is a classic interactive game developed using C++ and SFML. The player controls a snake, collects food, increases the snake's length and score, and avoids collisions with the game boundaries and itself.

The project is developed following the Software Development Life Cycle (SDLC) and Agile methodology, with emphasis on modular design, testing, version control, code quality, and maintainability.

## Planned Features

- Start and play a new game
- Control the snake using keyboard controls
- Random food generation
- Snake growth after eating food
- Score tracking
- Collision detection
- Game-over detection
- Restart functionality
- Increasing game difficulty

## Technology Stack

- **Programming Language:** C++
- **Graphics Library:** SFML
- **Version Control:** Git
- **Repository:** GitHub
- **Testing:** Google Test (gtest)
- **CI/CD:** Jenkins
- **Containerization:** Docker
- **Project Management:** Jira / GitHub Projects
- **Static Code Analysis:** SonarQube

## Project Structure

The project will follow a modular architecture to improve maintainability, testing, debugging, and future extension.

## Development Methodology

The project follows **Agile methodology** with iterative development, user stories, backlog management, Git branches, pull requests, code reviews, testing, and continuous integration.

## Collision Module (Ved)

The collision module implements the SAD Section 4.3.2 contract for FR-12 and
FR-13 and supplies the collision result needed by FR-14. It uses integer grid
cells and does not require SFML. The game controller and playable game are not
implemented by this module.

### Build and run the collision tests

Requirements: a C++17 compiler, CMake 3.20 or newer, Git, and a CMake-compatible
build tool. The first configuration downloads the pinned GoogleTest v1.17.0
source and requires internet access.

```sh
cmake -S . -B build -DBUILD_GAME=OFF -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
cmake -E chdir build ctest -C Debug --output-on-failure
```

To build only the library without downloading GoogleTest, configure with
`-DBUILD_TESTING=OFF`. These commands build the collision library and tests;
the movement-only game is built separately as described below.

### Integration contract

Include `collision_detector.h` and link the `snake_collision` CMake target.
Types are in namespace `snakegame`: `Cell`, `Body`, `BoardBounds` and
`CollisionType`. Pass the **current** body ordered head-to-tail, the candidate
next head, positive board dimensions, and whether that move will grow the snake.

```cpp
snakegame::CollisionDetector detector;
snakegame::Body body{{5, 4}, {4, 4}, {3, 4}};
const auto result = detector.check({6, 4}, body, {10, 8}, false);
// result is CollisionType::None: the candidate is free and inside the board.
```

On each movement tick, the controller must calculate the next head and food/growth
flag **before advancing the snake**. If the result is `Boundary` or `Self`, enter
game over and leave the body and score unchanged. Otherwise commit the move,
then apply food/scoring updates. Only the controller changes game state.

- Valid coordinates: `0 <= x < columns` and `0 <= y < rows`.
- On a non-growing move the tail leaves, so its cell is safe to enter.
- On a growing move the tail stays, so its cell remains occupied.
- The detector has no side effects and runs in O(snake length) time with O(1)
  extra space. Empty bodies and non-positive dimensions throw
  `std::invalid_argument`; initialization must validate them.

The `Snake` class exposes SFML rectangles through `getBody()`. For collision
integration, convert their pixel positions to the ordered integer grid cells
described in the SAD; pixel positions must not be passed as cell coordinates.
Rendering can convert cells to pixels. Game-over UI and game-loop integration
remain their respective owners' work.

Unit tests cover every wall, in-bounds edges, self-collision, both tail rules,
the TC-05 layouts, single-segment snakes, unchanged input and invalid setup.
Passing these tests verifies the detector; full TC-05 game-over display and
stopped-movement checks still need the integrated game.

## Build the movement game and both test suites

Install SFML 2.6.x for your compiler/architecture. SFML 3 uses a different API
and is not supported by this movement code. Set `SFML_DIR` to the directory
containing `SFMLConfig.cmake` if CMake cannot find the installation automatically.
No developer-specific installation path is hard-coded in the build.

```sh
cmake -S . -B build-game -DBUILD_GAME=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-game --config Debug
cmake -E chdir build-game ctest -C Debug --output-on-failure
```

Run `build-game/SnakeGame` with a single-configuration generator, or
`build-game/Debug/SnakeGame.exe` with Visual Studio. The current game demonstrates
arrow-key movement and window close only. Food, score, collision/game-over and
restart still need integration; building both modules does not connect them.
