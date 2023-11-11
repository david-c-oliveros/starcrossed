REM Build script for engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .cpp files.
SET cppFilenames=
FOR /R %%f in (*.cpp) do (
    SET cppFilenames=!cppFilenames! %%f
)

ECHO "Files:" %cppFilenames%

SET assembly=engine
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Iinclude\
SET linkerFlags=-luser32
SET defines=-D_DEBUG -DGEXPORT -D_CRT_SECURE_NO_WARNINGS

ECHO "Building %assembly%%..."
g++ %cppFilenames% %compilerFlags% -o ../bin/%assembly%.dll %defines% %includeFlags% %linkerFlags%
