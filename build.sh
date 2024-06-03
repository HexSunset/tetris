#!/bin/bash

set -xe

SRC="src"

BUILD="build"

NAME="tetris"

CCFLAGS="-Wall -Wextra -g"

INCLUDE="-lraylib -lm"

gcc $CCFLAGS $SRC/*.c -o $BUILD/$NAME $INCLUDE
