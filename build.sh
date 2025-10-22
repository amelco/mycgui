#!/bin/bash
set -xe

CFLAGS="-Wall -Wextra -g"
HEADERS="-I. -I./headers -I./src"
CFG="-fPIC -shared"
LIBS="-L. -L./bin -L./libraries -L./headers -l:libraylib.so -lm"

# examples/mg_test
#gcc $CFLAGS $HEADERS $CFG -o ./bin/libmg_test.so ./examples/mg_test_plug.c $LIBS -Wl,-rpath=./headers \
#    -march=native -ffast-math -flto=auto -lpthread
#gcc $CFLAGS $HEADERS -o ./bin/mg_test ./examples/mg_test.c $LIBS -l:libmg_test.so -Wl,-rpath=./libraries -Wl,-rpath=./bin -Wl,-rpath=./headers\
#    -march=native -ffast-math -flto=auto -lpthread

# examples/grid
gcc $CFLAGS $HEADERS $CFG -o ./bin/libgrid.so ./examples/grid_plug.c $LIBS -Wl,-rpath=./headers \
    -march=native -ffast-math -flto=auto -lpthread
gcc $CFLAGS $HEADERS -o ./bin/grid ./examples/grid.c $LIBS -l:libgrid.so -Wl,-rpath=./libraries -Wl,-rpath=./bin \
    -march=native -ffast-math -flto=auto -lpthread
