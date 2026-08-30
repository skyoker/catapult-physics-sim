cmake_minimum_required(VERSION 3.16)

# C is required for the glad loader (src/gl.c), CXX for everything else.
project(catapult_physics_sim C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate build/compile_commands.json so clangd resolves includes.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Place the binary at build/main (not build/src/main), which is what
# `statcode cpp cmake` runs.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Prefer the modern GLVND OpenGL libraries over the legacy ones.
cmake_policy(SET CMP0072 NEW)

# Target is named `main` to match `statcode cpp cmake` / <leader>rck.
add_executable(main
  src/main.cpp
  src/gl.c
)

# `include/` holds the vendored glad + KHR headers.
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(main PRIVATE
  OpenGL::GL
  glfw
  glm::glm
)
