#pragma once 
#include <vector>
#include "glad/gl.h"
#include <GLFW/glfw3.h>

struct EBO {
    GLuint ID;
    EBO() = default;
    EBO(std::vector<int> indices);
    EBO(std::vector<int> indices, GLenum usage);
    void Update(const std::vector<int>& indices);
    void Bind();
    void Unbind();
    void Delete();

};