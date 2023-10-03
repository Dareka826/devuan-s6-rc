#!/bin/sh

musl-clang \
    -Wall \
    -Wextra \
    -Werror \
    -static \
    ./trace_init.c \
    -o ./trace_init
