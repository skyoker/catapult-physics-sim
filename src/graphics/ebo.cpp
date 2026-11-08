#include "graphics/ebo.hpp"
#include "utl/logger.hpp"


EBO::EBO(std::vector<int> indices) {
    int lg_lv = 1;

    lg.log("Generating and writing EBO buffer...", true, lg_lv);
    glGenBuffers(1, &ID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
    lg.log("Generating and writing EBO buffer... DONE", false, lg_lv);

}

EBO::EBO(std::vector<int> indices, GLenum usage) {
    int lg_lv = 1;

    lg.log("Generating and writing EBO buffer...", true, lg_lv);
    glGenBuffers(1, &ID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), usage);
    lg.log("Generating and writing EBO buffer... DONE", false, lg_lv);

}

void EBO::Update(const std::vector<int>& indices) {
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);
    lg.log("Updated EBO contents", false, 3);

}

void EBO::Bind() {
    int lg_lv = 3;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    lg.log("Binded EBO as an ELEMENT_ARRAY_BUFFER", false, lg_lv);

}

void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    lg.log("Unbinded EBO", false, 3);

}

void EBO::Delete() {
    glDeleteBuffers(1, &ID);
    lg.log("Deleted EBO", false, 2);

}