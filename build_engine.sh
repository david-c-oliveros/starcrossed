#!/bin/bash
# Build script for engine

set echo on

mkdir -p ../bin

# Get a list of all the .cpp files
cppFilenames=$(find . -type f -name "*.cpp")

echo "Files:" $cppFilenames

assembly="game"
compilerFlags="-g -fPIC"
includeFlags="-Iengine/include/ -Igame/include/"
linkerFlags="-lxcb -lX11 -lX11-xcb -lxkbcommon -L/usr/X11R6/lib"
defines="-D_DEBUG"

echo "Building $assembly..."
echo g++ $cppFilenames $compilerFlags -o bin/$assembly $defines $includeFlags $linkerFlags
g++ $cppFilenames $compilerFlags -o bin/$assembly $defines $includeFlags $linkerFlags
