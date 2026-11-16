#!/usr/bin/env bash
# Automated evidence for TC-14 (SR-02, SR-03) — static source scan.
#
# SR-02: the game must not execute operating-system commands.
# SR-03: the game must not read or modify unrelated user files.
#
# The only file the game is allowed to open is its bundled font (read-only,
# sf::Font::loadFromFile in src/main.cpp). Anything else fails the build.
set -euo pipefail

SRC_DIR="${1:-src}"
status=0

check() {
    local label="$1" pattern="$2"
    local hits
    hits=$(grep -RInE "$pattern" "$SRC_DIR" --include='*.cpp' --include='*.h' || true)
    if [[ -n "$hits" ]]; then
        echo "FAIL [$label]"
        echo "$hits" | sed 's/^/    /'
        status=1
    else
        echo "PASS [$label]"
    fi
}

echo "== SR-02: no operating-system command execution =="
check "system()"            '(^|[^_[:alnum:]])system[[:space:]]*\('
check "popen()"             '(^|[^_[:alnum:]])_?popen[[:space:]]*\('
check "exec*() family"      '(^|[^_[:alnum:]])exec(l|lp|le|v|vp|vpe)[[:space:]]*\('
check "fork()/spawn()"      '(^|[^_[:alnum:]])(fork|posix_spawn|CreateProcess|ShellExecute|WinExec)[A-Za-z]*[[:space:]]*\('

echo "== SR-03: no writing / deleting user files =="
check "C stdio file open"   '(^|[^_[:alnum:]])(fopen|freopen|_wfopen)[[:space:]]*\('
check "output file streams" 'std::(ofstream|fstream)'
check "file removal/rename" '(^|[^_[:alnum:]])(remove|rename|unlink)[[:space:]]*\(|filesystem::(remove|rename|copy|create_directory)'
check "raw POSIX file I/O"  '(^|[^_[:alnum:]])(open|creat|write)[[:space:]]*\([^)]*O_'

echo "== SR-03: only the bundled font may be loaded =="
loads=$(grep -RInE 'loadFromFile' "$SRC_DIR" || true)
echo "$loads" | sed 's/^/    /'
if [[ -n "$loads" ]] && echo "$loads" | grep -vq 'font.loadFromFile'; then
    echo "FAIL [unexpected loadFromFile call]"; status=1
else
    echo "PASS [only font.loadFromFile is used]"
fi

if [[ $status -eq 0 ]]; then
    echo "Security scan passed: SR-02 and SR-03 static checks are clean."
fi
exit $status
