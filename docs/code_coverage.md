# Code coverage report

Line coverage counts executable source lines that ran at least once. This
report also shows how many functions ran.

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

| Source file | Executable lines | Functions |
| --- | ---: | ---: |
| [collision_detector.cpp](../src/collision_detector.cpp) | 16 / 16 (100.00%) | 1 / 1 (100.00%) |
| [food_manager.cpp](../src/food_manager.cpp) | 50 / 53 (94.34%) | 7 / 7 (100.00%) |
| [game_controller.cpp](../src/game_controller.cpp) | 88 / 113 (77.88%) | 14 / 18 (77.78%) |
| [main.cpp](../src/main.cpp) | 0 / 309 (0.00%) | 0 / 6 (0.00%) |
| [score_manager.cpp](../src/score_manager.cpp) | 30 / 30 (100.00%) | 7 / 7 (100.00%) |
| [snake.cpp](../src/snake.cpp) | 99 / 115 (86.09%) | 9 / 10 (90.00%) |
| **Gameplay and controller subtotal** | 283 / 327 (86.54%) | 38 / 43 (88.37%) |
| **All application source** | 283 / 636 (44.50%) | 38 / 49 (77.55%) |

## Reproduce the measurement

Configure a fresh Debug build with `BUILD_GAME=ON` and SFML 2.6.2. Apply
`-fprofile-instr-generate -fcoverage-mapping` to the production targets
`snake_collision`, `snake_food`, `snake_score`, `snake`,
`snake_controller` and `SnakeGame`. Link executables with
`-fprofile-instr-generate`. A local CMake project include applied these
compile options with `cmake_language(DEFER CALL ...)`; the repository
build files did not need to change.

Use a separate profile path for CMake's test discovery during the build.
Then run the following from the build directory. The five test executables
and the application supply the source coverage mappings. Only profiles from
CTest contribute execution counts.

```sh
mkdir -p profiles
LLVM_PROFILE_FILE="$PWD/profiles/%m-%p.profraw" ctest -C Debug --output-on-failure
xcrun llvm-profdata merge -sparse profiles/*.profraw -o tests.profdata
xcrun llvm-cov report -instr-profile=tests.profdata collision_tests \
  -object food_score_tests -object SnakeTests -object game_integration_tests \
  -object food_score_integration_tests -object SnakeGame.app/Contents/MacOS/SnakeGame \
  /path/to/SE-project/src/*.cpp
```

Use an empty `profiles/` directory for each run. The companion
[branch coverage report](branch_coverage.md) uses the same test run.
