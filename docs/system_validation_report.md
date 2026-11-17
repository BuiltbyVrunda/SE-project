# System Validation Report

The team recorded the manual and automated checks for TC-01 through TC-14 in [issue #10](https://github.com/BuiltbyVrunda/SE-project/issues/10) and the [Test Plan execution update](Snake_Game_Test_Plan_Execution_Update.pdf). The [traceability matrix](RTM.md) maps these cases to the SRS. The following records distinguish reported observations from checks that still need evidence.

| Test cases | Evidence and result |
| --- | --- |
| TC-01–TC-06 | Gameplay, keyboard movement, food, growth, score, wall collision and restart were reported passing. Controller tests cover all four walls, self collision, score preservation, and reset. [Collision integration validation](collision_integration_validation.md) |
| TC-07–TC-13 | The team reported manual results for closing, FPS, usability, repeat restarts, maintainability, compatibility and invalid input. The [execution update](Snake_Game_Test_Plan_Execution_Update.pdf) records the conditions and limitations. The final CI sanitizer run passed the automated suite. |
| TC-14 | **Static source scan passed. Runtime trace pending.** The [CI run](https://github.com/BuiltbyVrunda/SE-project/actions/runs/44013179973) passed the SR-02/SR-03 source scan. The Test Plan also calls for a file activity trace through one full round, restart and close. The execution update records that trace as unverified. |

The [two sprint recordings](demos/) show the core gameplay on the respective builds. They are scripted demos and do not include an OS-level file activity trace. To complete TC-14, capture the game process with Process Monitor on Windows or `strace` on Linux, review its file activity, and attach a short record of the platform, revision, actions, and findings.
