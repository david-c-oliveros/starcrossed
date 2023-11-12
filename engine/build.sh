#!/bin/bash
# Build script for engine

set echo on

mkdir -p ../bin

# Get a list of all the .cpp files
cppFilenames=$(find . -type f -name "*.cpp")

echo "Files:" $cppFilenames

assembly="engine"
compilerFlags="-g -shared -fPIC"
# -fms-extensions
# -Wall -Werror
includeFlags="-Iinclude"
linkerFlags="-lxcb -lX11 -lX11-xcb -lxkbcommon -L/usr/X11R6/lib"
defines="-D_DEBUG -DGEXPORT"

echo "Building $assembly..."
g++ $cppFilenames $compilerFlags -o ../bin/lib$assembly.so $defines $includeFlags $linkerFlags
