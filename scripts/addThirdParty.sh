#!/bin/bash

# this script sets up all dependent submodules
#set -x
PROJECT_DIR=$(pwd)
echo "$PROJECT_DIR"
if echo "$PROJECT_DIR" | grep -q -E '/OrderbookManager$'
then
  echo "begin to add submodules..."
else
	echo -e "\033[31mERROR: run the script under project's dir(OrderbookManager)\033[0m"
	exit 1
fi

MODULES=$(git submodule summary)
# cppint
CPPLINT="third_party/cpplint"
if [[ "$MODULES" == *"$CPPLINT"* ]]; then
  echo "Skipping $CPPLINT"
else
  cd "$PROJECT_DIR" || return
  git submodule add -f https://github.com/cpplint/cpplint "$CPPLINT"
  cd "$CPPLINT" || return
  git checkout 2a22afe
fi
# gtest
GTEST="third_party/gtest"
if [[ "$MODULES" == *"$GTEST"* ]]; then
  echo "Skipping $GTEST"
else
  cd "$PROJECT_DIR" || return
  git submodule add -f https://github.com/google/googletest.git "$GTEST"
  cd "$GTEST" || return
  git checkout 2fe3bd9
fi
# spdlog
SPDLOG="third_party/spdlog"
if [[ "$MODULES" == *"$SPDLOG"* ]]; then
  echo "Skipping $SPDLOG"
else
  cd "$PROJECT_DIR" || return
  git submodule add -f https://github.com/gabime/spdlog.git "$SPDLOG"
  cd "$SPDLOG" || return
  git checkout 10578ff
fi
# abseil-cpp
ABSL="third_party/abseil-cpp"
if [[ "$MODULES" == *"$ABSL"* ]]; then
  echo "Skipping $ABSL"
else
  cd "$PROJECT_DIR" || return
  git submodule add -f https://github.com/abseil/abseil-cpp.git "$ABSL"
  cd "$ABSL" || return
  git checkout 2151058
fi
