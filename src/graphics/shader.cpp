#include "graphics/shader.hpp"


Shader::Shader(const char* vshader, const char* fshader) {
    int lg_lv = 1;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, lg_lv, &vshader, nullptr);
    lg.log("created ID for shader and added vertex shader source", false, 1);

    lg.log("Compiling vertex shader...", true, lg_lv);
    glCompileShader(vertexShader);

    GLint v_success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &v_success);
    if (!v_success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        lg.log_err(std::string("Vertex shader compile error: ") + infoLog);
    }
    lg.log("Compiling vertex shader... DONE", false, lg_lv);


    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fshader, nullptr);

    lg.log("created ID for shader and added fragment shader source", false, lg_lv);

    lg.log("Compiling fragment shader...", true, lg_lv);
    glCompileShader(fragmentShader);

    GLint f_success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &f_success);
    if (!f_success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        lg.log_err(std::string("Vertex shader compile error: ") + infoLog);
    }
    lg.log("Compiling fragment shader... DONE", false, lg_lv);


    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    lg.log("Attached and linked program", false, lg_lv);


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    lg.log("Delted now useless shaders", false, lg_lv);


}

void Shader::Delete() {
    int lg_lv = 2;
    glDeleteProgram(ID);
    lg.log("Deleted shader", false, lg_lv);

}

void Shader::Activate() {
    int lg_lv = 3;
    glUseProgram(ID);
    lg.log("Activated shader", false, lg_lv);

}
