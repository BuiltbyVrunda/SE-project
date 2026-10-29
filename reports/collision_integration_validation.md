# Collision integration check

Checked on 29 October 2026 against merged main commit `feb30bc`, with the
additional integration tests in this change.

Environment: macOS arm64, Apple Clang 21, CMake 4.1 and locally built SFML 2.6.2.
Debug build, with assertions enabled.

## Results

The game and test targets built successfully. CTest reported **44/44 passed**:
23 collision cases, 20 controller/food cases, and the movement test executable.

| Check | Result |
| --- | --- |
| All four walls cause GameOver before advancing the snake | Pass |
| Self-collision causes GameOver without advancing | Pass |
| Steps, elapsed time and direction input cannot move a game-over snake | Pass |
| Fatal collision preserves a non-zero earned score and the last valid body | Pass |
| Restart resets score, body, direction, collision state and food placement | Pass |
| Gameplay continues after restart | Pass |
| Entering the actual departing-tail cell without growing is safe | Pass |

The existing tail-integration test took extra leftward steps and entered a free
cell rather than the tail cell. It now uses the short up-left-down route and
asserts that the candidate head equals the current tail before checking the move.
Four wall-direction cases and a scored collision/restart case were also added.
No gameplay implementation was changed.

Commands (after configuring SFML_DIR or CMAKE_PREFIX_PATH for SFML 2.6.2):

```sh
cmake -S . -B build-game -DBUILD_GAME=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-game --config Debug
cmake -E chdir build-game ctest -C Debug --output-on-failure
```

## Visual check still needed

The graphical launch crashed in SFML's macOS display initialization before the
ready screen appeared. A local dependency diagnostic also reported no suitable
pixel format. Its temporary change was reverted and is not part of this PR.
The automated results above used the original SFML 2.6.2 build.

The score value used by the renderer is checked, but the final-score text and
keyboard-driven restart have not been visually verified on this Mac. Do not mark
full manual TC-05 as passed from these automated results alone.

On a working graphical test machine, collect food, hit a wall or the body, check
that Game Over displays the earned score and the snake remains still, then press
R and confirm the reset round and zero score. Record that result separately.
