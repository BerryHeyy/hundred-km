#pragma once

#include <GLFW/glfw3.h>
#include <mat4x4.hpp>

namespace player
{
    void init(int swidth, int sheight);

    const glm::mat4& get_view_matrix();

    void handle_movement(GLFWwindow *window, float delta_time);
    void handle_mouse(GLFWwindow* window, double xpos, double ypos);

    void update(GLFWwindow *window, float delta_time);

    void regenerate_view_matrix();
}
