#!/bin/bash
set -xe

gcc -g -o ./bin/$1 -I./include -I./src -I./lib ./examples/$1.c -L./lib -L./include -l:libraylib.a -lm
