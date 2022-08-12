#!/bin/sh
DIFF="`git diff --staged -U0 | clang-format-diff -p1`"
if [ -z "${DIFF}" ]; then
	exit 0
else
	echo "[WARNING] some files are not property formatted. Consider running: git diff --staged -U0 | clang-format-diff -p1 -i"
	exit 1
fi