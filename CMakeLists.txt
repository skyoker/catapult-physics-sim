cmake_minimum_required(VERSION 3.10)

project(Atom3D)

# `src` is on the include path so headers under src/physics resolve from any source file,
# not just the ones that happen to sit next to them
include_directories(include src)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fix OpenGL warning
cmake_policy(SET CMP0072 NEW)

add_executable(Atom3D
    src/main.cpp
    src/graphics/vbo.cpp
    src/graphics/vao.cpp
    src/graphics/shader.cpp
    src/graphics/engine.cpp
    src/graphics/gl.c
    src/graphics/logger.cpp
    src/graphics/ebo.cpp 
    src/graphics/mesh.cpp
    src/graphics/mesh3d.cpp
    src/graphics/notemesh.cpp
    src/graphics/camera.cpp
    src/graphics/scene.cpp
    src/tui/tui.cpp
)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Curses REQUIRED)

target_include_directories(Atom3D PRIVATE ${CURSES_INCLUDE_DIRS})

target_link_libraries(Atom3D
    OpenGL::GL
    glfw
    glm::glm
    ${CURSES_LIBRARIES}
)