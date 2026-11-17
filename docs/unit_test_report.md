# Unit Test Report

The final coverage run passed **73 of 73 CTest entries** across five test executables. This is the combined unit and integration count. The unit tests cover the collision detector, food placement, scoring, and snake movement. The controller and food/scoring integration suites exercise those modules together.

| Unit test area | What it verifies | Evidence |
| --- | --- | --- |
| Collision | All four walls, self collision, the departing-tail rule, and invalid board or body inputs | [Collision tests](../tests/collision_detector_test.cpp) |
| Food and scoring | Food placement on free cells, full-board handling, food detection, and score updates | [Food and scoring tests](../tests/food_score_test.cpp) |
| Movement | Grid movement, direction changes, reversal rejection, and growth | [Movement tests](../tests/snake_test.cpp) |

The coverage run used a Debug build with Apple Clang 21 and SFML 2.6.2 on macOS arm64. The [code coverage report](code_coverage.md) and [branch coverage report](branch_coverage.md) give the revision, commands, and results. The [earlier CTest log](collision_test_run_2026-11-07.txt) records the 7 November validation run. The latest [GitHub Actions run](https://github.com/BuiltbyVrunda/SE-project/actions/runs/44013179973) passed Linux and macOS builds, sanitizer checks, static analysis, and the security source scan.
