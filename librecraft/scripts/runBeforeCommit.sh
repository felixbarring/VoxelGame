#!/bin/sh

bash format.sh

echo
echo ============================================
echo Building with GCC ==========================
echo ============================================
echo

source useGcc.sh
bash setupBuildRelease.sh
bash build.sh

echo
echo ============================================
echo Building with Clang ========================
echo ============================================
echo

source useClang.sh
bash setupBuildRelease.sh
bash build.sh
