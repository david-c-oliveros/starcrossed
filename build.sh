#! /bin/sh

if [[ -z $1 ]]; then
    echo "Building for Linux"
    cd out/build ; make

elif [ $1 = "em" ]; then
    echo "Building for the web with emscripten"
    cd out/build ; ../../external/emsdk/upstream/emscripten/emmake make

fi
