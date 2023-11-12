#!/bin/bash
# Build script for game

set echo on

mkdir -p ../bin

#Get a list of all the .cpp files
cppFilenames=$(find . -type f -name "*.cpp")

echo "Files:" $cppFilenames

assembl="game"
compilerFlags="-g -fPIC"
# -fms-extensions
# -Wall -Werror
includeFlags="-Iinclude -I../engine/include/"
linkerFlags="-L../bin -lengine -Wl,-rpath,."
defines="-D_DEBUG -DGIMPORT"

echo "Building $assembly..."
echo g++ $cppFilenames $compilerFlags -o ../bin/$assembly $defines $includeFlags $linkerFlags
g++ $cppFilenames $compilerFlags -o ../bin/$assembly $defines $includeFlags $linkerFlags
