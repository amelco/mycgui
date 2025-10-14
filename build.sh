#!/bin/bash
set -xe

#CFLAGS="-Wall -Wextra -g"
CFLAGS="-g"
HEADERS="-I. -I./headers -I./src"
CFG="-fPIC -shared"
LIBS="-L. -L./bin -L./libraries -l:libraylib.so -lm"

#gcc $CFLAGS $HEADERS -o ./bin/mg_test ./examples/mg_test.c $LIBS -l:libgrid.so -Wl,-rpath=./libraries -Wl,-rpath=./bin

gcc $CFLAGS $HEADERS $CFG -o ./bin/lib$1.so ./examples/$1_plug.c $LIBS \
    -march=native -ffast-math -flto=auto -lpthread
gcc $CFLAGS $HEADERS -o ./bin/$1 ./examples/$1.c $LIBS -l:libgrid.so -Wl,-rpath=./libraries -Wl,-rpath=./bin \
    -march=native -ffast-math -flto=auto -lpthread
