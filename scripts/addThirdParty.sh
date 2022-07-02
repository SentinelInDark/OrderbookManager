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
  cd "$DIR" || return
  echo "begin to clone cpplint"
  git submodule add -f https://github.com/cpplint/cpplint "$CPPLINT"
  cd "$CPPLINT" || return
  git checkout 1.6.0
fi

# gtest
GTEST="third_party/gtest"
if [[ "$MODULES" == *"$GTEST"* ]]; then
  echo "Skipping $GTEST"
else
  cd "$DIR" || return
  echo "begin to clone gtest"
  git submodule add -f https://github.com/google/googletest.git "$GTEST"
  cd "$GTEST" || return
  git checkout v1.12.0
fi

# spdlog
SPDLOG="third_party/spdlog"
if [[ "$MODULES" == *"$SPDLOG"* ]]; then
  echo "Skipping $SPDLOG"
else
  cd "$DIR" || return
  echo "begin to clone spdlog"
  git submodule add -f https://github.com/gabime/spdlog.git "$SPDLOG"
  cd "$SPDLOG" || return
  #git checkout 76fb40d95455f249bd70824ecfcae7a8f0930fa3
  git checkout  v1.9.2
fi
