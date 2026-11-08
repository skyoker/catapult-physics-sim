#include "graphics/vbo.hpp"

VBO::VBO(std::vector<GLfloat> vertices) {
    int lg_lv = 1;

    lg.log("Generating and writing VBO buffer...", true, lg_lv);
    glGenBuffers(1, &ID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    vertex_count = vertices.size();
    lg.log("Generating and writing VBO buffer... DONE", false, lg_lv);


}

VBO::VBO(std::vector<GLfloat> vertices, GLenum usage) {
    int lg_lv = 1;

    lg.log("Generating and writing VBO buffer...", true, lg_lv);
    glGenBuffers(1, &ID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), usage);
    vertex_count = vertices.size();
    lg.log("Generating and writing VBO buffer... DONE", false, lg_lv);

}

void VBO::Update(const std::vector<GLfloat>& vertices) {
    Bind();
    // orphan the old store first so the driver does not stall on in-flight draws
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    vertex_count = vertices.size();
    lg.log("Updated VBO contents", false, 3);

}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    lg.log("Binded VBO", false, 3);

}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    lg.log("Unbinded VBO", false, 3);

}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
    lg.log("Deleted VBO", false, 2);

}
