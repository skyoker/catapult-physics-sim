#pragma once
#include "glad/gl.h"
#include <vector>
#include "utl/logger.hpp"

struct VBO {
    GLuint ID;

    int vertex_count;

    VBO() = default;
    VBO(std::vector<GLfloat> vertices);
    VBO(std::vector<GLfloat> vertices, GLenum usage);
    void Update(const std::vector<GLfloat>& vertices);
    void Bind();
    void Unbind();
    void Delete();
};
