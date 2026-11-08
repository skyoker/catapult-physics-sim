#include "graphics/mesh3d.hpp"
#include "utl/logger.hpp"

Mesh3D::Mesh3D(const std::vector<GLfloat>& vertices, const std::vector<int>& indices, const std::vector<float>& colors, GLenum draw_mode) : draw_mode(draw_mode) {
    int lg_lv = 1;
    lg.log("Generating 3D mesh...", true, lg_lv);

    this->vertices = vertices;
    this->indices = indices;

    this->meshVAO = VAO();
    meshVAO.Bind();

    this->meshVBO = VBO(AddVertexAttributes(3, 4, vertices, colors));
    this->meshEBO = EBO(indices);

    meshVAO.LinkAttrib(meshVBO, 0, 3, 0, 7 * sizeof(GLfloat));
    meshVAO.LinkAttrib(meshVBO, 1, 4, 3 * sizeof(GLfloat), 7 * sizeof(GLfloat));

    buffers_ready = true;
    lg.log("Generating 3D mesh... DONE", false, lg_lv);
}

void Mesh3D::update(const std::vector<GLfloat>& vertices, const std::vector<int>& indices, const std::vector<float>& colors) {
    this->vertices = vertices;
    this->indices = indices;

    std::vector<GLfloat> interleaved = AddVertexAttributes(3, 4, vertices, colors);

    meshVAO.Bind();

    if (!buffers_ready) {
        // first upload: create the buffers as dynamic and wire up the attributes
        meshVBO = VBO(interleaved, GL_DYNAMIC_DRAW);
        meshEBO = EBO(indices, GL_DYNAMIC_DRAW);
        meshVAO.LinkAttrib(meshVBO, 0, 3, 0, 7 * sizeof(GLfloat));
        meshVAO.LinkAttrib(meshVBO, 1, 4, 3 * sizeof(GLfloat), 7 * sizeof(GLfloat));
        buffers_ready = true;
    } else {
        meshVBO.Update(interleaved);
        // the EBO must be bound with the VAO active so the VAO records it
        meshVAO.Bind();
        meshEBO.Update(indices);
    }

    lg.log("Updated Mesh3D geometry", false, 3);
}

void Mesh3D::Bind() {
    meshVAO.Bind();
    lg.log("Binded Mesh3D VAO", false, 3);
}

void Mesh3D::draw() {
    glDrawElements(draw_mode, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    lg.log("Drawn Mesh3D", false, 3);
}

std::vector<GLfloat> Mesh3D::AddVertexAttributes(int attrA_size, int attrB_size, const std::vector<float>& A, const std::vector<float>& B) {
    int vertex_count = A.size() / attrA_size;
    std::vector<GLfloat> combined;
    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < attrA_size; j++)
            combined.push_back(A[i * attrA_size + j]);
        for (int j = 0; j < attrB_size; j++)
            combined.push_back(B[i * attrB_size + j]);
    }
    return combined;
}
