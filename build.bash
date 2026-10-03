#!/usr/bin/env bash
# Build script for Lin.
#
# IMPORTANT: run this with the MSYS2 bash, not WSL.
# `bash` on PATH resolves to C:\Windows\System32\bash.exe (WSL), where the
# MSYS2 toolchain is not visible and Windows paths do not resolve.
#
#   From PowerShell:
#     C:\msys64\usr\bin\bash.exe -lc "cd ~/Lin && ./build.bash"
#   From MSYS2 Mintty (project root):
#     ./build.bash

set -euo pipefail

CXX="${CXX:-g++}"
STD="${STD:-c++20}"
SUBSYSTEM="${SUBSYSTEM:-console}"
BUILD_DIR="build"
OUT="$BUILD_DIR/lin"

# UNICODE/_UNICODE are required: main.cpp uses L"..." literals with
# RegisterClass/CreateWindowEx. Without them those calls resolve to the *A
# variants and the wchar_t arguments fail to compile.
DEFINES=(-D_WIN32 -DUNICODE -D_UNICODE)
WARNINGS=(-Wall -Wextra)
LIBS=(-lkernel32 -luser32 -lgdi32)

cd "$(dirname "$0")"

# Normalise PATH so this script works no matter how bash was launched.
# `bash -lc` on MSYS2 can come up with an empty PATH, and the Windows-side PATH
# (where mingw64\bin lives) is not always inherited. Prefer a real MinGW
# toolchain over MSYS2's own /usr/bin/g++, which lacks proper windows.h support.
if [ -z "${PATH:-}" ]; then
  export PATH="/usr/bin:/bin"
fi
for prefix in /ucrt64 /mingw64 /clang64 /clangarm64 /usr; do
  if [ -x "$prefix/bin/$CXX" ] || [ -x "$prefix/bin/$CXX.exe" ]; then
    export PATH="$prefix/bin:$PATH"
    break
  fi
done
if ! command -v "$CXX" >/dev/null 2>&1; then
  echo "error: '$CXX' not found on PATH." >&2
  echo "       Set it explicitly, e.g.: CXX=/mingw64/bin/g++.exe $0" >&2
  exit 1
fi

# Native (Windows-style) root for clangd; MSYS paths are not resolvable by it.
ROOT="$(pwd -W 2>/dev/null || pwd)"
ROOT="${ROOT//\\//}"

mapfile -t SOURCES < <(find src -name '*.cpp' | sort)
if [ ${#SOURCES[@]} -eq 0 ]; then
  echo "error: no .cpp files found under src/" >&2
  exit 1
fi

mkdir -p "$BUILD_DIR"

echo "==> compiling (${#SOURCES[@]} source file(s), $CXX -std=$STD -m$SUBSYSTEM)"
OBJECTS=()
COMPILE_COMMANDS=()
for src in "${SOURCES[@]}"; do
  obj="$BUILD_DIR/$(printf '%s' "${src%.cpp}" | tr '/' '_').o"
  printf '  CXX  %s\n' "$src"
  "$CXX" "${DEFINES[@]}" "${WARNINGS[@]}" "-std=$STD" "-m$SUBSYSTEM" -c "$src" -o "$obj"
  OBJECTS+=("$obj")
  COMPILE_COMMANDS+=("$CXX ${DEFINES[*]} ${WARNINGS[*]} -std=$STD -m$SUBSYSTEM -c $src -o $obj")
done

echo "==> linking"
printf '  LD   %s.exe\n' "$OUT"
"$CXX" "-m$SUBSYSTEM" "${OBJECTS[@]}" "${LIBS[@]}" -o "$OUT"

# Generated rather than hand-written so it can never drift from the real build.
# All paths are relative (resolved against "directory") or use forward slashes,
# so the JSON needs no backslash escaping.
{
  printf '[\n'
  for i in "${!SOURCES[@]}"; do
    [ "$i" -gt 0 ] && printf ',\n'
    printf '  {\n'
    printf '    "directory": "%s",\n' "$ROOT"
    printf '    "command": "%s",\n' "${COMPILE_COMMANDS[$i]}"
    printf '    "file": "%s"\n' "${SOURCES[$i]}"
    printf '  }'
  done
  printf '\n]\n'
} > compile_commands.json

echo "==> wrote compile_commands.json (clangd / Helix LSP)"
echo "built: $OUT.exe"