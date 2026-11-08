#pragma once
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include "utl/utils.hpp"
#include <glm/glm.hpp>

struct OrbitCamera {
    vec3 target;
    float yaw;
    float pitch;
    float distance;
    float fov;
    float near;
    float far;
    float min_distance = 0.1f;

    OrbitCamera() : target{0,0,0}, distance(15.0f), yaw(0), pitch(0), fov(70.0f), near(0.01f), far(100.0f) { lg.log("Initing OrbitCamera vars... DONE", false, 1); }
    OrbitCamera(vec3 target, float distance, float fov = 70.0f, float near = 0.01f, float far = 100.0f);

    glm::mat4 view_matrix() const;
    glm::mat4 projection_matrix(float aspect) const;
    vec3 get_position() const;
    void handle_input(GLFWwindow* window);
};
