cmake_minimum_required(VERSION 3.27.4)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

add_compile_options(-Wfatal-errors -w -O2 -g)

find_package(OpenGL)

project(App)

add_executable(${PROJECT_NAME}
    external/glad/src/glad.c
    src/main.cpp
    game/src/game.cpp
    engine/src/stb_image.cpp
    engine/src/renderer.cpp
    engine/src/entity.cpp
    engine/src/tile_world.cpp
    engine/src/camera.cpp
    engine/src/object.cpp
    engine/src/model.cpp
    engine/src/mesh.cpp
    engine/src/shader.cpp
    engine/src/audio.cpp
    )

add_subdirectory(external/assimp)

include_directories(".")

target_include_directories(${PROJECT_NAME}
    PUBLIC engine/include
    PUBLIC game/include
    PUBLIC external/glad/include
    PUBLIC external/glm
    PUBLIC external/assimp/include
    )

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    )

target_link_libraries(${PROJECT_NAME} glfw3 ${GLFW_LIBRARIES}
    m
    dl
    pthread
    assimp
    )

    #set_target_properties(${PROJECT_NAME}
    #    PROPERTIES SUFFIX ".html"
    #    LINK_FLAGS " --bind --emrun \
    #    -s FORCE_FILESYSTEM \
    #    -s FULL_ES3=1 \
    #    -s EXPORT_ALL=1 \
    #    -s MALLOC=emmalloc \
    #    -s WASM=1 \
    #    -s ALLOW_MEMORY_GROWTH=1 \
    #    -Wemcc \
    #    -s MIN_WEBGL_VERSION=2 \
    #    -s USE_LIBPNG=1 \
    #    -s USE_WEBGL2=1 \
    #    -s USE_GLFW=3 \
    #    --preload-file ../../res \
    #    --preload-file ../../shaders")
