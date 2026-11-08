#pragma once
#include <vector>
#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "ebo.hpp"
#include "vbo.hpp"
#include "vao.hpp"
#include "shader.hpp"

struct Mesh3D {
    std::vector<GLfloat> vertices;
    std::vector<int> indices;

    VAO meshVAO;
    VBO meshVBO;
    EBO meshEBO;

    GLenum draw_mode;

    // false until the VBO/EBO exist. A default-constructed Mesh3D only has a VAO, so
    // dynamic meshes (NoteMesh) create their buffers on the first update() instead.
    bool buffers_ready = false;

    Mesh3D() : draw_mode(GL_TRIANGLES) {}
    Mesh3D(const std::vector<GLfloat>& vertices, const std::vector<int>& indices, const std::vector<float>& colors, GLenum draw_mode = GL_TRIANGLES);

    void Bind();
    void draw();
    // Re-upload geometry into the existing buffers, creating them on first call.
    void update(const std::vector<GLfloat>& vertices, const std::vector<int>& indices, const std::vector<float>& colors);

    std::vector<GLfloat> AddVertexAttributes(int attrA_size, int attrB_size, const std::vector<float>& A, const std::vector<float>& B);
};
