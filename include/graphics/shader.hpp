#pragma once
#include "glad/gl.h"
#include "utl/logger.hpp"

struct Shader {
    GLuint ID;

    Shader() : ID(0) {}
    Shader(const char* vshader, const char* fshader);
    void Delete();
    void Activate();
};
