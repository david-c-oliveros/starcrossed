#! /bin/sh

args=""
for file in "$@"; do
    args+=" src/$file.cpp include/$file.h"
done

vim -p $args
