#include "graphics/engine.hpp"
#include <iostream>


Engine2D::Engine2D(int sx, int sy) {
    int lg_lv = 1;

    lg.log("glfw init", true, lg_lv);
    glfwInit();
    lg.log("glfw init success", false, lg_lv);
    std::vector<GLfloat> nextp;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    lg.log("window hints created", false, lg_lv);


    window = glfwCreateWindow(sx, sy, "OpenGL", nullptr, nullptr);
    lg.log("created window", false, lg_lv);


    this->sx = sx;
    this->sy = sy;

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    lg.log("Set context to current", false, lg_lv);

    glfwSwapInterval(1);
    lg.log("Set fps to 60", false, lg_lv);




    gladLoadGL(glfwGetProcAddress);
    lg.log("Loaded opengl from GLAD", false, lg_lv);


    int fb_w, fb_h;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    glViewport(0, 0, fb_w, fb_h);
    lg.log("Made viewport", false, lg_lv);


    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    lg.log("Cleared all color", false, lg_lv);

}

// --- Engine3D ---

Engine3D::Engine3D(int sx, int sy) {
    int lg_lv = 1;

    lg.log("glfw init", true, lg_lv);
    glfwInit();
    lg.log("glfw init success", false, lg_lv);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    lg.log("window hints created", false, lg_lv);

    window = glfwCreateWindow(sx, sy, "OpenGL", nullptr, nullptr);
    lg.log("created window", false, lg_lv);

    this->sx = sx;
    this->sy = sy;

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    lg.log("Set context to current", false, lg_lv);

    glfwSwapInterval(1);
    lg.log("Set fps to 60", false, lg_lv);

    gladLoadGL(glfwGetProcAddress);
    lg.log("Loaded opengl from GLAD", false, lg_lv);

    int fb_w, fb_h;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    glViewport(0, 0, fb_w, fb_h);
    lg.log("Made viewport", false, lg_lv);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lg.log("Cleared color and depth", false, lg_lv);
}

void Engine3D::update_viewport() {
    int fb_w, fb_h;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    glViewport(0, 0, fb_w, fb_h);
    lg.log("Updated viewport", false, 3);
}

void Engine3D::update_frame() {
    int lg_lv = 3;

    lg.log("swapping buffers and polling events...", true, lg_lv);
    update_viewport();
    glfwSwapBuffers(window);
    glfwPollEvents();
    lg.log("swapping buffers and polling events... DONE", false, lg_lv);
    lg.log("clearing frame...", true, lg_lv);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lg.log("clearing frame... DONE", false, lg_lv);
}

// --- Engine2D ---

void Engine2D::update_viewport() {
    int fb_w, fb_h;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    glViewport(0, 0, fb_w, fb_h);
    lg.log("Updated viewport", false, 3);

}

void Engine2D::run(const Shader &shader, const VBO &vbo, const VAO &vao) {
    int lg_lv = 3;

    lg.log("starting frame...", true, lg_lv);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    lg.log("drawing vbo GL_TRIANGLES", false, lg_lv);
    glDrawArrays(GL_TRIANGLES, 0, vbo.vertex_count);

    lg.log("swapping buffers and polling events...", true, lg_lv);
    glfwSwapBuffers(window);
    glfwPollEvents();
    lg.log("swapping buffers and polling events... DONE", false, lg_lv);


    this->update_viewport();
}

void Engine2D::update_frame() {
    int lg_lv = 3;

    lg.log("swapping buffers and polling events...", true, lg_lv);
    update_viewport();
    glfwSwapBuffers(window);
    glfwPollEvents();
    lg.log("swapping buffers and polling events... DONE", false, lg_lv);
    lg.log("clearing frame...", true, lg_lv);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    lg.log("clearing frame... DONE", false, lg_lv);

}
