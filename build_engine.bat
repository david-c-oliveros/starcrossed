REM Build script for engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .cpp files.
REM SET cppFilenames=
REM FOR /R %%f in (*.cpp) do (
REM     SET cppFilenames=!cppFilenames! %%f
REM )

SET cppFilenames="engine/src/*.cpp game/src/*.cpp"

ECHO "Files:" %cppFilenames%

SET assembly=game
SET compilerFlags=-g -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=^
-Iengine/include^
-Igame/include^
-Iexternal/glad/include/^
-Iexternal/glfw/include/

SET linkerFlags=
-luser32
-Lexternal/glfw/build/src -lglfw

SET defines=-D_DEBUG

ECHO "Building %assembly%%..."
g++ %cppFilenames% external/glad/src/glad.c %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%
