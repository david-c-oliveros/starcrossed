#!/bin/bash
# Build script for engine

set echo on

mkdir -p ../bin

# Get a list of all the .cpp files
cppFilenames=$(find . -type d -path ./external/glm -prune -o -type f -name "*.cpp" -print)

echo "Files:" $cppFilenames

assembly="game"
compilerFlags="-g -fPIC"

includeFlags="
-Iengine/include/
-Igame/include/
-Iexternal/glad/include/
-Iexternal/glfw-3.3.8/include/
-Iexternal/glm/
"

linkerFlags="
-lxcb -lX11 -lX11-xcb -lxkbcommon -L/usr/X11R6/lib
-Lexternal/glfw/build/src -lglfw
"

defines="-D_DEBUG"

echo "Building $assembly..."
echo g++ $cppFilenames external/glad/src/glad.c $compilerFlags -o build/bin/$assembly $defines $includeFlags $linkerFlags
g++ $cppFilenames external/glad/src/glad.c $compilerFlags -o build/bin/$assembly $defines $includeFlags $linkerFlags
