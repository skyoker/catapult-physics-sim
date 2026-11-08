#pragma once
#include <vector>
#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "ebo.hpp"
#include "vbo.hpp"
#include "vao.hpp"
#include "shader.hpp"





struct Mesh2D {
    std::vector<GLfloat> vertices;
    std::vector<int> indices;

    VAO meshVAO;
    VBO meshVBO;
    EBO meshEBO;

    // uniform locations
    GLint offsetLoc;
    GLint scaleLoc;
    GLint colorLoc;

    // local mesh values
    float ofs_x;
    float ofs_y;

    float scl_x;
    float scl_y;

    // other
    bool set_defualt = false;

    //contructor
    Mesh2D() = default;
    Mesh2D(const std::vector<GLfloat> &vertices, const std::vector<int> &indices, const std::vector<float> &colors);

    // methods
    void Bind(Shader &shader);
    std::vector<GLfloat> AddVertexAtrributes(int atribA_size, int atribB_size, const std::vector<float> &A, const std::vector<float> &B);

    // basic functionality
    void draw();
    void translate(float x, float y);
    void scale(float x, float y);
    void set_color(float r, float g, float b, float a);

    // apply - apply scale, translations etc.. and maybe colors

};