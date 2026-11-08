#include "graphics/camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

static float g_scroll_offset = 0.0f;

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        yoffset *= 0.1;
    g_scroll_offset = (float)yoffset;
}

OrbitCamera::OrbitCamera(vec3 target, float distance, float fov, float near, float far)
    : target(target), distance(distance), fov(fov), near(near), far(far) {
    int lg_lv = 1;
    lg.log("Initing OrbitCamera vars...", true, lg_lv);
    yaw = 0.0f;
    pitch = 0.0f;
    lg.log("Initing OrbitCamera vars... DONE", false, lg_lv);
}

glm::mat4 OrbitCamera::view_matrix() const {
    vec3 pos = get_position();
    glm::mat4 view = glm::lookAt(
        glm::vec3(pos.x, pos.y, pos.z),
        glm::vec3(target.x, target.y, target.z),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    lg.log("Computed view matrix", false, 3);
    return view;
}

glm::mat4 OrbitCamera::projection_matrix(float aspect) const {
    lg.log("Computed projection matrix", false, 3);
    return glm::perspective(glm::radians(fov), aspect, near, far);
}

vec3 OrbitCamera::get_position() const {
    float pitch_rad = glm::radians(pitch);
    float yaw_rad = glm::radians(yaw);
    vec3 pos;
    pos.x = target.x + distance * cos(pitch_rad) * sin(yaw_rad);
    pos.y = target.y + distance * sin(pitch_rad);
    pos.z = target.z + distance * cos(pitch_rad) * cos(yaw_rad);
    return pos;
}

void OrbitCamera::handle_input(GLFWwindow* window) {
    static bool callbacks_set = false;
    if (!callbacks_set) {
        glfwSetScrollCallback(window, scroll_callback);
        callbacks_set = true;
    }

    static bool first_mouse = true;
    static double last_x, last_y;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (first_mouse) {
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }
        float dx = (float)(xpos - last_x);
        float dy = (float)(ypos - last_y);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            dx *= 0.1f; dy *= 0.1f;
        }
        yaw -= dx * 0.3f;
        pitch += dy * 0.3f;
        pitch = std::clamp(pitch, -89.0f, 89.0f);
        last_x = xpos;
        last_y = ypos;
    } else {
        first_mouse = true;
    }

    distance *= powf(1.1f, -g_scroll_offset);
    if (distance < min_distance) distance = min_distance;
    g_scroll_offset = 0.0f;
}
