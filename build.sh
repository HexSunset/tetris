#!/bin/bash

set -xe

SRC="src"
INCLUDE="-lraylib -lm"

BUILD="build"

NAME="tetris"

CCFLAGS="-Wall -Wextra -g"

gcc $CCFLAGS $SRC/*.c -o $BUILD/$NAME $INCLUDE
