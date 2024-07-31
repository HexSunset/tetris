#!/bin/bash

SRC="src"

BUILD="build"

NAME="tetris"

CCFLAGS="-Wall -Wextra -g"

INCLUDE="-lraylib -lm"

echo "+ Building '$BUILD/$NAME'..."
gcc $CCFLAGS $SRC/*.c -o $BUILD/$NAME $INCLUDE
echo "+ Built '$BUILD/$NAME'!"

if [[ $1 == "run" ]]; then
    echo "+ Running '$BUILD/$NAME'"
    $BUILD/$NAME
    echo "+ Finished running '$BUILD/$NAME'"
fi
