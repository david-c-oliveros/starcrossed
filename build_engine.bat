REM Build script for engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .cpp files.
SET cppFilenames=
FOR /R ./engine %%f in (*.cpp) do (
    SET cppFilenames=!cppFilenames! %%f
)

FOR /R ./game %%f in (*.cpp) do (
    SET cppFilenames=!cppFilenames! %%f
)

SET cppExternalFilenames=external/glad/src/glad.c external/imgui/backends/imgui_impl_glfw.cpp external/imgui/backends/imgui_impl_opengl3.cpp external/imgui/imgui*.cpp

ECHO "Files:" %cppFilenames%

SET assembly=game
SET compilerFlags=-g -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Iengine/include -Igame/include -Iexternal/glad/include/ -Iexternal/glfw-3.3.8/include/ -Iexternal/glm -Iexternal/imgui/ -Iexternal/imgui/backends

SET linkerFlags=^
 -luser32^
 -Lexternal/glfw-3.3.8/build/windows/src -lglfw3^
 -lgdi32^
 -lopengl32

SET defines=-D_DEBUG

ECHO "Building %assembly%%..."
g++ %cppFilenames% %cppExternalFilenames% %compilerFlags% -o out/build/bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%
