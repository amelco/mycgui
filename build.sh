#!/bin/bash

set -xe

# gcc -g -O3 -o test mg_test.c -I raylib/build/raylib/include -L raylib/build/raylib -l:libraylib.a -lm

#gcc -g -O3 -o $1 $1.c -I raylib/build/raylib/include -L raylib/build/raylib -l:libraylib.a -lm

gcc -g -O3 -o $1 $1.c -I ./ -L ./ -l:libraylib.a -lm
