#! /bin/sh

if [[ -z $1 ]]; then
    echo "Configuring for Linux"
    cmake -S . -B out/build

elif [[ $1 = "em" ]]; then
    echo "Configuring for the web with emscripten"
    external/emsdk/upstream/emscripten/emcmake cmake -S . -B out/build
fi
