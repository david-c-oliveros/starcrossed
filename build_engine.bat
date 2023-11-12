REM Build script for engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .cpp files.
SET cppFilenames=
FOR /R %%f in (*.cpp) do (
    SET cppFilenames=!cppFilenames! %%f
)

ECHO "Files:" %cppFilenames%

SET assembly=game
SET compilerFlags=-g -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Iengine/include -Igame/include
SET linkerFlags=-luser32
SET defines=-D_DEBUG

ECHO "Building %assembly%%..."
g++ %cppFilenames% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%
