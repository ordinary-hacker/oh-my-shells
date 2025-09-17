#!/bin/bash
# Cross-compile oh-my-shells for multiple platforms using zig cc

set -e

# Gather all source files as in Makefile
SRCDIR="src"
LIBDIR="lib/tomlc17/src"
OUTDIR="release/builds"
SOURCES=( $SRCDIR/*.c $LIBDIR/tomlc17.c )
CFLAGS="-std=c11"

# List of targets: (zig triple format)
TARGETS=(
  "x86_64-linux-gnu"
  "aarch64-linux-gnu"
  "arm-linux-gnueabihf"
  "x86_64-macos"
  "aarch64-macos"
  "x86_64-freebsd"
  "x86_64-netbsd"
)

# These are all I expect to support
# If the community wants more then pls someone add them for me lol

# Create output directory
mkdir -p "$OUTDIR"

# Compile for each target

echo "Compiling for multiple targets using zig cc..."
for TARGET in "${TARGETS[@]}"; do
  case $TARGET in
    *windows*) EXT=".exe" ;;
    *) EXT="" ;;
  esac
  OUTFILE="$OUTDIR/oh-my-shells-$TARGET$EXT"
  echo "  -> $TARGET ($OUTFILE)"
  zig cc -target $TARGET -O2 "${SOURCES[@]}" -o "$OUTFILE" $CFLAGS
done

echo "All done! Binaries are in $OUTDIR."
