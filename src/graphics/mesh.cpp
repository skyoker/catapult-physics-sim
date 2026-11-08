#include "graphics/mesh.hpp"
#include "utl/logger.hpp"
#include "utl/utils.hpp"


Mesh2D::Mesh2D(const std::vector<GLfloat> &vertices, const std::vector<int> &indices, const std::vector<float> &colors) {
    int lg_lv = 1;

    if (!vertices.size() == indices.size() * 2) {
        lg.log_war("Too many or to little indices or vertices for Mesh!");
    }
    if (!vertices.size() / 2 == colors.size() * 4) {
        lg.log_war("Too many or to little colors compared to vertices! Remember to use monocolor after then");
    }

    this->vertices = vertices;
    this->indices = indices;

    lg.log("Copied verts, indices and colors to mesh instance as feilds", false, lg_lv);

    this->meshVAO = VAO();
    meshVAO.Bind();

    this->meshVBO = VBO(AddVertexAtrributes(2, 4, vertices, colors));
    this->meshEBO = EBO(indices);
    lg.log("Generated VBO, VAO and EBO and assigned them to mesh instance feilds", false, lg_lv);

    meshVAO.LinkAttrib(meshVBO, 0, 2, 0, 6 * sizeof(GLfloat));        // position
    meshVAO.LinkAttrib(meshVBO, 1, 4, 2 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // color
    

}

void Mesh2D::Bind(Shader &shader) {
    shader.Activate();
    meshVAO.Bind();

    this->offsetLoc = glGetUniformLocation(shader.ID, "u_offset");
    this->scaleLoc = glGetUniformLocation(shader.ID, "u_scale");
    this->colorLoc = glGetUniformLocation(shader.ID, "u_color");

    if (!set_defualt) {
        this->scale(1.0f, 1.0f);   // natural size
        this->translate(0.0f, 0.0f); // centered
        set_defualt = true;
    }

    lg.log("Binded mesh and retived uniform locations", false, 2);
}

void Mesh2D::draw() {

    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    lg.log("Drawn mesh", false, 3);
    
}

void Mesh2D::translate(float x, float y) {
    glUniform2f(offsetLoc, x, y);

}

void Mesh2D::scale(float x, float y) {
    glUniform2f(scaleLoc, x, y);

}

void Mesh2D::set_color(float r, float g, float b, float a) {
    glUniform4f(colorLoc, r, g, b, a);
}

std::vector<GLfloat> Mesh2D::AddVertexAtrributes(int atribA_size, int atribB_size, const std::vector<float> &A, const std::vector<float> &B) {
    int vertex_count = A.size() / atribA_size;
    std::vector<GLfloat> combined;
    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < atribA_size; j++)
            combined.push_back(A[i * atribA_size + j]);
        for (int j = 0; j < atribB_size; j++)
            combined.push_back(B[i * atribB_size + j]);
    }
    return combined;
}
