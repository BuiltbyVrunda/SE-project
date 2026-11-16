# Coding Standards — Snake Game

These rules are enforced (or reported) automatically by CI where possible.

| Rule | Why | Enforced by |
|---|---|---|
| C++17, no compiler extensions (`CMAKE_CXX_EXTENSIONS OFF`) | Portability across GCC / Clang / MSVC (NFR-05) | CMake |
| Zero warnings under `-Wall -Wextra -Wpedantic` | Warnings often hide real bugs | CI `build-and-test` (`-Werror`) |
| No memory errors / undefined behaviour | Defensive & secure coding (SR-05) | CI `sanitizers` (ASan + UBSan) |
| No cppcheck warnings | Static analysis catches defects early | CI `static-analysis` |
| No OS command execution, no file writes | Security requirements SR-02 / SR-03 | CI `security-scan` |
| Formatting per `.clang-format` (4 spaces, Allman braces, 100 cols) | Readability, smaller diffs | CI (advisory report) |

## Design rules
1. **Separate logic from presentation.** Domain classes (`CollisionDetector`, `FoodManager`,
   `ScoreManager`) must not include SFML, so they can be unit-tested headlessly.
2. **Pure functions where possible.** `CollisionDetector::check` has no side effects and does not
   mutate the snake.
3. **Validate inputs at module boundaries.** Invalid board sizes throw `std::invalid_argument`
   or return `PlacementResult::InvalidBounds`; nothing silently continues with bad data.
4. **Use the type system.** `enum class` for states and results (`GameState`, `CollisionType`,
   `PlacementResult`); `[[nodiscard]]` on queries whose result must not be ignored;
   `std::optional<Cell>` for "maybe no food".
5. **No magic numbers in logic.** Named constants (`DefaultPointsPerFood = 10`, `CELL_SIZE`,
   movement interval `0.15f`).
6. **Deterministic tests.** Randomness goes through an injectable `std::mt19937` so tests can fix
   the seed.
7. **Every behaviour change ships with a test** and an RTM update if it touches a requirement.

## Naming
- Types: `PascalCase` (`GameController`). Functions/variables: `camelCase` (`getNextHeadGrid`).
- Constants: `PascalCase` for `static constexpr` members, `UPPER_SNAKE_CASE` for file-local constants.
- Files: `snake_case.cpp/.h`, one main class per file pair.
- Domain code lives in `namespace snakegame`. (`Snake` predates this rule and is still in the global namespace and depends on SFML types — recorded as technical debt in `docs/Sprint_Report.md`.)
