# SAD review

Reviewed by Ved Mudkavi on 31 October 2026 for issue #8.

Reviewed the [SAD](../docs/SAD_SnakeGame.docx), its three diagrams and the
implementation on `main` at `feb30bc`. The Word file is unchanged from the
previously rendered and checked version. This report records how the proposed
design maps to the implemented game and what still needs final review.

**Result:** the main gameplay flow matches the design. The review is complete
with the differences and open findings below; this is not a claim that every
SRS requirement or manual test has passed.

## Architecture and interfaces

| SAD responsibility | Implementation | Review result |
| --- | --- | --- |
| Input Controller | Event switch in `src/main.cpp` | Same responsibility, implemented as main-loop code rather than a separate class. |
| Renderer and SFML adapter | Drawing, window and font code in `src/main.cpp` | Presentation is separate from collision/food checks, but there is no Renderer class or snapshot object. |
| Game Controller | `src/game_controller.h` and `.cpp` | Owns score, state, board and timer. Coordinates movement, food, collisions and restart. |
| Snake | `src/snake.h` and `.cpp` | Stores SFML rectangles in pixels; `getGridBody()` and `getNextHeadGrid()` convert them to integer cells. This differs from the proposed pure grid-only model. |
| Food Manager | `src/food_manager.h` and `.cpp` | Free-cell placement, optional food, injected RNG overload and BoardFull handling match the contract. |
| Collision Detector | `src/collision_detector.h` and `.cpp` | Matches the proposed `check(nextHead, body, bounds, grows)` contract and departing-tail rule. |

API names were adapted: `requestDirection()` is `changeDirection()`, `nextHead()`
is `getNextHeadGrid()`, and `body()` is `getGridBody()`. `reset()` uses the fixed
initial layout rather than taking a body argument. The controller uses
`startRound()`, `restart()`, `handleDirectionInput()`, `update()` and getters
instead of a command enum and `snapshot()`.

Rendering currently reads the controller's data without changing it, but mutable
Snake and FoodManager getters are public for test setup. The interface therefore
does not enforce the read-only snapshot boundary proposed in the SAD.

## Diagrams and gameplay flow

The component diagram remains a logical responsibility diagram. Input and
rendering boxes correspond to code in `main.cpp`, not separately implemented
classes. Snake also depends on SFML, unlike the intended domain isolation.

Both sequence diagrams agree with `GameController::step()`:

1. Calculate the candidate head without advancing.
2. Determine whether the move would eat food and grow.
3. Check boundary and self-collision with that growth flag.
4. On collision, set GameOver and preserve the body and score.
5. Otherwise advance, add ten points if food was eaten, and place replacement food.
6. Enter RoundComplete if placement reports a full board; render the current state.

The diagram message names are conceptual; the exact implemented names are mapped
above. The diagrams abbreviate the collision arguments; the API section and code
include `BoardBounds` explicitly.

Ready, Playing, GameOver and RoundComplete are implemented. Closing is handled
by `window.close()` rather than a GameState enum member. Restart resets the
body, direction/input latch, score, food, timer and collision reason.

## Settings and security

The actual game uses an 800x600 window, 20-pixel cells and a **40x30 grid** rather
than the proposed 30x20 default. It starts with three segments at cells
(5,5), (4,5), (3,5), moving right. The zero score, ten points per food and 150 ms
movement interval agree with the proposed settings. These values are fixed in
code, not a player-editable configuration.

Enter or Space starts; arrows steer; R restarts after game over or completion;
Escape or the close button exits. Space and Escape extend the proposed controls.
The score HUD is inside the play area, whereas the SAD proposed placing it
outside the grid. Confirm its readability during the final manual check.

The reviewed game source has no network/database code, input-derived paths or
shell/process execution calls. It allows supported keys through a switch and
validates direction/state before movement. These source checks support the
security design but do not replace full TC-13/TC-14 execution and evidence.

C++17, CMake, GoogleTest and GitHub Actions are implemented. Local integration
checks used SFML 2.6.2; CMake accepts SFML 2 and CI installs the Ubuntu-packaged
version. The final environment record should distinguish these rather than
claiming one exact patch version across all machines. Docker, Jenkins and
SonarQube are not runtime design requirements and are not assumed configured.

## Findings for the final integration review

| SAD section | Finding and required follow-up |
| --- | --- |
| 4.1 and 4.4 | The controller constructor does not validate positive bounds, whether the initial body fits, or a positive finite movement interval. Placement errors at start/reset are ignored. Validate initialization before claiming the designed configuration-failure handling is implemented. The current executable supplies fixed valid defaults. |
| 4.4 | On font-load failure, the game tries fixed system-font paths and then continues without text. The SAD specifies clean termination if required graphics resources fail. Resolve this difference: missing text would hide controls, score and Game Over information. |
| 4.4 | There is no application-level exception handler. Closing the window does not immediately leave the current frame; update/render code still follows. Review these paths against the promised safe shutdown/error handling. |
| 3.1, 3.7 and 3.8 | A 60 FPS cap is not the required measured 60-second performance result. The available restart unit checks are not evidence of ten consecutive manual restart cycles. Record TC-08 and TC-10 separately. |

These findings remain for shared integration review **#11** and Test Plan review
**#7/#9**. The original SAD is retained as the submitted design baseline; this
report is the implementation-difference record for final documentation/demo.
They are not silently treated as approved design changes or resolved code fixes.

The SAD has the required architecture pattern, component descriptions,
security discussion, requirement mappings, two sequence diagrams, interfaces,
error-handling and UX sections. Its traceability table includes all 18 FRs,
6 NFRs and 5 SRs. Actual acceptance results belong in the Test Plan/RTM.

## Follow-up on 1 November 2026

The later main revision `0615642` adds a separate `ScoreManager`, food/scoring
tests, an in-memory best score and the FPS display. GameController now delegates
current score, best score and food count to its owned ScoreManager. Best score
survives a round reset but is not written to disk; record it as an addition to
the original design scope. The pre-movement collision/food/growth sequence is
unchanged.

This review branch now packages the font in the native Mac app, resolves its
fixed paths relative to the executable and terminates if no font loads. This
addresses the font-failure finding above. The initialization/exception/close-path
findings are not claimed fixed. See `collision_integration_validation.md` for
the 73-check result and the newly confirmed wall/Game Over/restart screen checks.
