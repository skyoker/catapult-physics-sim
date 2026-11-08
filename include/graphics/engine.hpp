#pragma once
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "vbo.hpp"
#include "vao.hpp"
#include "utl/logger.hpp"

struct Engine2D {
    GLFWwindow* window;
    int sx;
    int sy;

    Engine2D(int sx, int sy);
    void update_viewport();
    void run(const Shader &shader, const VBO &vbo, const VAO &vao);
    void update_frame();
};

struct Engine3D {
    GLFWwindow* window;
    int sx;
    int sy;

    Engine3D(int sx, int sy);
    void update_viewport();
    void update_frame();
};
