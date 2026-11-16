# Contributing

## Branching model
- `main` is always releasable and protected by CI.
- Each member works on a personal branch: `charan`, `vrunda`, `ved`, `tushar`
  (short-lived feature branches such as `feature/food-placement` are also fine).
- Before pushing: `git pull origin main` into your branch, resolve conflicts, rebuild, re-test.

## Workflow (one backlog item at a time)
1. Pick an issue from the GitHub Projects board and move it to **In Progress**.
2. Write or update the test first where practical (TDD), then the code.
3. Build and run all tests locally:
   ```sh
   cmake -S . -B build -DSNAKE_ENABLE_WARNINGS=ON
   cmake --build build
   ctest --test-dir build --output-on-failure
   ```
4. Open a Pull Request to `main` using the PR template; reference the issue (`Closes #NN`).
5. CI must be green and **one teammate other than the author must approve** before merging.
6. Merge, then move the card to **Done**.

## Commit messages
Imperative mood, ≤ 72-character subject, reference the issue where relevant:
```
Add departing-tail rule to collision check (closes #18)
```

## Coding standards
See [`docs/CODING_STANDARDS.md`](docs/CODING_STANDARDS.md). Format with `clang-format -i <file>`.

## Releasing
Update `CHANGELOG.md`, then:
```sh
git tag v1.0.0
git push origin v1.0.0
```
