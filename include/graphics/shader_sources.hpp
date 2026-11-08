const char* defaultVertextShader_src = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "uniform vec2 u_offset;\n"
    "uniform vec2 u_scale;\n"
    "out vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x * u_scale.x + u_offset.x, aPos.y * u_scale.y + u_offset.y, aPos.z, 1.0);\n"
    "   vColor = aColor;\n"
    "}\0";

//Fragment Shader source code
const char* defaultFragmentShader_src = "#version 330 core\n"
    "in vec4 vColor;\n"
    "out vec4 FragColor;\n"
    "uniform vec4 u_color = vec4(1.0);\n"
    "void main()\n"
    "{\n"
    "   FragColor = vColor * u_color;\n"
    "}\n\0";

const char* vertex3D_src = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "out vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);\n"
    "   vColor = aColor;\n"
    "}\n\0";

const char* fragment3D_src = "#version 330 core\n"
    "in vec4 vColor;\n"
    "out vec4 FragColor;\n"
    "uniform vec4 u_color = vec4(1.0);\n"
    "void main()\n"
    "{\n"
    "   FragColor = vColor * u_color;\n"
    "}\n\0";
