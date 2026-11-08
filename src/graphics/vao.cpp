#include "graphics/vao.hpp"

VAO::VAO() {
    int lg_lv = 1;

    glGenVertexArrays(1, &ID);
    lg.log("Generated VAO", false, lg_lv);

}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, int size, int offset, int stride) {
    int lg_lv = 2;

    lg.log("Linking VAO to VBO...", true, lg_lv);
    Bind();
    vbo.Bind();
    glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride, (void*)(intptr_t)offset);
    glEnableVertexAttribArray(layout);
    vbo.Unbind();
    lg.log("Linking VAO to VBO... DONE", false, lg_lv);

}

void VAO::Bind() {
    glBindVertexArray(ID);
    lg.log("Binded VAO as a Vertex array", false, 3);

}

void VAO::Unbind() {
    glBindVertexArray(0);
    lg.log("Unbinded VAO", false, 3);

}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
    lg.log("Deleted VAO", false, 2);

}
