# Snake Movement Validation Report

## Automated Tests

| Test                             | Result |
| -------------------------------- | ------ |
| Initial direction                | PASS   |
| Valid direction change           | PASS   |
| Direct reversal rejection        | PASS   |
| Rapid consecutive turn rejection | PASS   |
| Invalid direction rejection      | PASS   |
| Forward movement                 | PASS   |

**Automated tests:** 6/6 passed.

## Manual Tests

| Test                         | Result |
| ---------------------------- | ------ |
| Continuous movement          | PASS   |
| Arrow-key controls           | PASS   |
| Opposite-direction rejection | PASS   |
| Unsupported keyboard input   | PASS   |
| Window closing               | PASS   |

## Environment

* Language: C++
* Build system: CMake
* Compiler: Microsoft Visual C++
* Graphics library: SFML 2.6.2

## Conclusion

The project built successfully, all six automated tests passed, and the manual movement checks were completed.
