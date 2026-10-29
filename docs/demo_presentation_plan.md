# Snake Game Demonstration and Recording Plan

**Project:** Software Engineering Mini-Project - Snake Game  
**Target Delivery:** Issue #19 Integration Demo & Sprint 2 Submission  
**Team Members:** Yele Charan (PES1UG25CS856), Vrunda C (PES1UG24CS541), Ved Mudkavi (PES1UG24CS522), Tushar N (PES1UG24CS504)  

---

## 1. Demo Roles and Responsibilities Distribution

To ensure fair coordination across the team:

| Team Member | Module / Responsibility | Demonstration Focus |
| :--- | :--- | :--- |
| **Yele Charan** | Snake Movement | Launching game, initial Ready state, continuous steering using Arrow Keys, reversal prevention |
| **Vrunda C** | Food Placement & Scoring | Food generation, snake eating food, growth from 3 to 4+ segments, HUD score updating (+10/food) |
| **Ved Mudkavi / Tushar N** | Collision Detection | Triggering boundary wall collision and self-collision, Game Over screen, final score display |
| **Tushar N** | Integration & Lifecycle | Clean restart ('R' key) with zero state leakage, running 39 automated tests via CTest, showing GitHub Actions CI status |

---

## 2. Step-by-Step Demo Script & Recording Order

| Step | Action | Presenter | What is Visible on Screen | Expected Outcome |
| :---: | :--- | :--- | :--- | :--- |
| **1** | Run executable: `SnakeGame.exe` | Charan | 800x600 window with bounded border, 3-segment snake at (5, 5), red food item, overlay: "SNAKE GAME", "Press ENTER or SPACE to Start" | Game launches smoothly, Ready state displayed, snake is static |
| **2** | Press `ENTER` or `SPACE` | Charan | Overlay disappears, score HUD appears: "Score: 0", snake starts moving right at 150ms per step | Game transitions to `Playing` state |
| **3** | Steer snake using Arrow Keys | Charan | Snake turns Up, Left, Down, Right smoothly; attempts to press immediate opposite key (e.g. Left while moving Right) are ignored | Responsive 4-way movement, reversal rejection active |
| **4** | Guide snake into food | Vrunda | Snake head reaches red food circle; snake grows by 1 segment; HUD score increments to "Score: 10"; replacement food appears immediately on an unoccupied cell | Food consumption, snake growth, score increment, replacement food |
| **5** | Eat 2 more food items | Vrunda | Snake grows to length 5; score increments to 20 and then 30 | Multiple food pickups work reliably |
| **6** | Trigger fatal Wall Collision | Ved / Tushar | Steer snake straight into the right boundary wall | Gameplay stops instantly; Game Over overlay appears with "GAME OVER", "Wall Collision!", "Final Score: 30", "Press 'R' to Restart" |
| **7** | Demonstrate movement stoppage | Ved / Tushar | Pressing arrow keys does not move the snake or alter the final score | Strict GameOver guard prevents movement after death |
| **8** | Press `R` to restart | Tushar | Game immediately resets to Playing: snake returns to 3 segments at initial position, moving right, score resets to 0, food is spawned at a free cell | Immediate restart, no state leakage from round 1 |
| **9** | Trigger Self-Collision | Ved / Tushar | Eat 2 food items (length 5), turn Up, Left, Down into the snake's own body | Instant detection: Game Over overlay displays "Self-Collision!" with the new final score |
| **10** | Run automated tests | Tushar | Open terminal: `ctest --test-dir build --output-on-failure --verbose` | 39/39 tests pass (100% pass rate) in ~3 seconds |
| **11** | Show GitHub Actions CI | Tushar | Open GitHub repository Actions tab: show green checkmark on latest commit | Automated CI verified on push and pull request |

---

## 3. Demo Preparation Checklist

Before recording or presenting live:
- [x] Build executable with `cmake --build build --config Release`.
- [x] Verify that `sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll`, and `libstdc++-6-x64.dll` are in the executable directory.
- [x] Verify font loading (bundled `assets/arial.ttf` loads without warnings).
- [x] Run `ctest --test-dir build --output-on-failure` to ensure all 39 tests pass.
- [x] Confirm screen recorder captures both game audio/video and console terminal.

