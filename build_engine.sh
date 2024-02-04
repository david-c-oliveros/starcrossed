#!/bin/bash
# Build script for engine

set echo on

mkdir -p ../bin

# Get a list of all the .cpp files
#cppEngineFilenames=$(find . -type d -path ./external/glm -prune -o -type f -name "*.cpp" -print)
cppEngineFilenames=$(find engine -type f -name "*.cpp" -print)
cppGameFilenames=$(find game -type f -name "*.cpp" -print)
cppExternalFilenames="
external/glad/src/glad.c
external/imgui/backends/imgui_impl_glfw.cpp
external/imgui/backends/imgui_impl_opengl3.cpp
external/imgui/imgui*.cpp
"


assembly="game"
compilerFlags="-g -fPIC -Wfatal-errors -w"

includeFlags="
-Iengine/include/
-Igame/include/
-Iexternal/glad/include/
-Iexternal/glfw-3.3.8/include/
-Iexternal/glm/
-Iexternal/imgui/
-Iexternal/imgui/backends/
-Iexternal/glText/
"

linkerFlags="
-lxcb -lX11 -lX11-xcb -lxkbcommon -L/usr/X11R6/lib
-Lexternal/glfw/build/src -lglfw
"

defines="-D_DEBUG"

echo "Building $assembly..."
echo g++ $cppGameFilenames $cppEngineFilenames $cppExternalFilenames $compilerFlags -o out/build/bin/$assembly $defines $includeFlags $linkerFlags
g++ $cppGameFilenames $cppEngineFilenames $cppExternalFilenames $compilerFlags -o out/build/bin/$assembly $defines $includeFlags $linkerFlags
