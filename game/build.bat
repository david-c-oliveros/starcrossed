REM Build script for game
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .cpp files.
SET cppFilenames=
FOR /R %%f in (*.cpp) do (
    SET cppFilenames=!cppFilenames! %%f
)

ECHO "Files:" %cppFilenames%

SET assembly=game
SET compilerFlags=-g
REM -Wall -Werror
SET includeFlags=-Iinclude -I../engine/include/
SET linkerFlags=-L../bin/ -lengine.lib
SET defines=-D_DEBUG -DIMPORT

ECHO "Building %assembly%%..."
g++ %cppFilenames% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%
