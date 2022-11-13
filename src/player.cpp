#include "player.hpp"

#include <gtc/matrix_transform.hpp>

Player::Player()
{
    camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    campera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera_right = glm::normalize(glm::cross(campera_front, camera_up));

    movement_speed = 0.05f;

    regenerate_view_matrix();
}

void Player::update(GLFWwindow *window)
{
    // Handle Input
    bool inputted = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_pos += campera_front * movement_speed;
        inputted = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_pos -= campera_front * movement_speed;
        inputted = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_pos -= camera_right * movement_speed;
        inputted = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_pos += camera_right * movement_speed;
        inputted = true;
    }

    if (inputted)
    {
        regenerate_view_matrix();
    }
}

void Player::regenerate_view_matrix()
{
    view_matrix = glm::lookAt(camera_pos, camera_pos + campera_front, camera_up);
}

