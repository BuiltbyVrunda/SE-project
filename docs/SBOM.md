# Software Bill of Materials (human-readable)

A machine-readable SPDX SBOM (`SnakeGame-sbom.spdx.json`) is generated automatically and attached to
every GitHub Release by `.github/workflows/release.yml`. This page summarises it.

| Component | Version | How it is obtained | Licence | Used for |
|---|---|---|---|---|
| SnakeGame (this project) | 1.0.0 | source | — (course project) | the game |
| SFML (graphics, window, system) | 2.5.1 on the Ubuntu 22.04 CI runner, 2.6.2 on Windows (team test machine) and macOS Homebrew | system package / Homebrew / prebuilt | zlib/png | window, input, rendering, timing |
| GoogleTest | 1.17.0 (commit `52eb810`) | CMake `FetchContent`, pinned | BSD-3-Clause | unit / integration tests (test-only) |
| C++ standard library | toolchain | compiler | toolchain | containers, `<random>`, `<filesystem>` |
| `assets/arial.ttf` | — | committed to repo | **Proprietary (Microsoft)** | HUD text |

## Known risk
`arial.ttf` is a Microsoft font and is not licensed for redistribution. For a public release it should be
replaced with an open font such as **DejaVu Sans** (Bitstream Vera licence) or **Roboto** (Apache-2.0).
The code already falls back to `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf` on Linux.
