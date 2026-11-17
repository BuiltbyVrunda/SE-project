# Branch coverage report

Branch coverage counts the true and false outcomes exercised for the source
conditions tracked by LLVM. Short-circuit conditions can be tracked separately.

Measured on 17 November 2026 against source revision
[`6b274d1`](https://github.com/BuiltbyVrunda/SE-project/commit/6b274d1fc2a84a15ffa42bb0394730107e8858ed).
The Debug build used Apple Clang and LLVM 21.0.0, CMake 4.1.0, SFML 2.6.2,
and GoogleTest 1.17.0 on macOS 26.6.2 (arm64). CTest passed **73 of 73 entries**
across the existing five test executables.

The scope is the six application `.cpp` files in `src/`. Test sources,
headers, and third-party code are outside these totals. The full total includes
`main.cpp`, whose window, keyboard, and drawing paths were not run by this
automated suite. The gameplay and controller subtotal excludes `main.cpp`.
Manual gameplay and the demo recordings did not contribute profile data.

| Source file | Branch outcomes |
| --- | ---: |
| [collision_detector.cpp](../src/collision_detector.cpp) | 20 / 20 (100.00%) |
| [food_manager.cpp](../src/food_manager.cpp) | 23 / 28 (82.14%) |
| [game_controller.cpp](../src/game_controller.cpp) | 12 / 20 (60.00%) |
| [main.cpp](../src/main.cpp) | 0 / 74 (0.00%) |
| [score_manager.cpp](../src/score_manager.cpp) | 4 / 4 (100.00%) |
| [snake.cpp](../src/snake.cpp) | 30 / 38 (78.95%) |
| **Gameplay and controller subtotal** | 89 / 110 (80.91%) |
| **All application source** | 89 / 184 (48.37%) |

The automated suite exercised all **20 of 20** tracked outcomes in the collision
detector. The largest unexercised set is in the GUI entry point, followed by
the controller and movement code. See the [code coverage report](code_coverage.md)
for the reproducible build and test commands. Add `-show-branch-summary` to
`llvm-cov report` to display branch totals, or use `llvm-cov show
-show-branches=count` with the same profile, binaries and source paths to
inspect individual outcomes.
