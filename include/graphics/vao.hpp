#pragma once
#include "glad/gl.h"
#include "graphics/vbo.hpp"
#include "utl/logger.hpp"

struct VAO {
    GLuint ID;

    VAO();
    void LinkAttrib(VBO& vbo, GLuint layout, int size, int offset, int stride);
    void Bind();
    void Unbind();
    void Delete();
};
