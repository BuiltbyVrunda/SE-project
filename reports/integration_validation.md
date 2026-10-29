# Integration Validation Report

**Date:** 29 October 2026  
**Primary Integration Task:** Issue #19 (Integrate playable game, display and lifecycle)  
**Assigned Engineer / Integrator:** Tushar N (SRN: PES1UG24CS504)  
**Contributors:** Yele Charan (Movement), Vrunda C (Food & Scoring), Ved Mudkavi (Collisions), Tushar N (Lifecycle & Integration)  

---

## 1. Automated Test Execution Summary

All tests were executed using CTest on the clean build directory.

| Test Target | Suite / Area | Tests Run | Passed | Failed | Status |
| :--- | :--- | :---: | :---: | :---: | :---: |
| `collision_tests` | Wall collisions, valid cells, self collisions, tail departure rules, invalid bounds | 23 | 23 | 0 | **PASS** |
| `SnakeTests` | Charan's snake movement, turns, reversals, forward advancement | 6 | 6 | 0 | **PASS** |
| `game_integration_tests` | 12 required scenarios, FoodManager unit tests, GameController lifecycle | 15 | 15 | 0 | **PASS** |
| **Total** | **CTest Full Test Suite** | **44** | **44** | **0** | **PASS (100%)** |

---

## 2. Verification of the 12 Integration Scenarios

| Scenario # | Description | Verified Condition | Result |
| :---: | :--- | :--- | :---: |
| **1** | Normal snake movement | Head advances by 1 discrete grid cell per tick in heading direction | **PASS** |
| **2** | Movement in permitted directions | Steering Up, Down, Left, and Right executes properly | **PASS** |
| **3** | Prevent invalid reverse movement | Immediate 180° reversal rejected; rapid multi-key turn reversals rejected by latch | **PASS** |
| **4** | Wall collision | Moving into all 4 board bounds stops game and enters GameOver | **PASS** |
| **5** | Self-collision | Looping snake body into occupied non-tail segment triggers Self-collision | **PASS** |
| **6** | Food consumption and scoring | Reaching food cell awards 10 points, increases length by 1, spawns replacement food | **PASS** |
| **7** | Game Over behavior | State transitions to GameOver; final score and reason displayed; input halted | **PASS** |
| **8** | Prevention of movement after Game Over | Further ticks do not modify snake coordinates or score | **PASS** |
| **9** | Restart after Game Over | Pressing 'R' transitions state back to Playing cleanly | **PASS** |
| **10** | Reset of all gameplay state | Snake resets to 3 segments at (5,5), direction resets to Right, score resets to 0 | **PASS** |
| **11** | Continued gameplay after restart | Game can be steered, food collected, and score accumulated after restart | **PASS** |
| **12** | Collision regression testing | TC-05 departing tail rule verified: entering tail cell is safe only on non-growing move | **PASS** |

---

## 3. Manual Graphical Verification

| Feature | Observable Expected Behavior | Observed Result | Status |
| :--- | :--- | :--- | :---: |
| **Start Screen** | "SNAKE GAME", controls guide, prompt to press Enter/Space | Rendered clearly with centered overlay | **PASS** |
| **Gameplay HUD** | "Score: X" visible in top-left during gameplay | Accurately reflects food eaten (+10 per food) | **PASS** |
| **Food Rendering** | Red circular food placed strictly on unoccupied grid cells | Visible, distinct from snake | **PASS** |
| **Snake Rendering** | Yellow head segment, green body segments | Distinguishable head, clean segment spacing | **PASS** |
| **Game Over Screen** | "GAME OVER", collision reason, "Final Score: X", "Press 'R' to Restart" | Board state preserved underneath overlay | **PASS** |
| **Restart Action** | Pressing 'R' clears overlay and starts fresh round immediately | Instant reset, no state leakage | **PASS** |
| **Window Close** | Pressing ESC or clicking window 'X' closes window gracefully | Process exits with code 0, no leaks | **PASS** |

---

## 4. Build & Environment Details

- **Language:** C++17
- **Compiler:** GCC 14.2.0 (x86_64-w64-mingw32) / MSVC compatible
- **Build System:** CMake 3.20+ with Ninja
- **Graphics Library:** SFML 2.6.2
- **Testing Frameworks:** Google Test v1.17.0 (via FetchContent) & CTest
- **CI/CD:** GitHub Actions (`.github/workflows/ci.yml`)

