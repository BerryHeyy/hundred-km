#include "player.hpp"

#include <gtc/matrix_transform.hpp>

Player::Player()
{
    camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    campera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera_right = glm::normalize(glm::cross(campera_front, camera_up));

    movement_speed = 2.0f;

    regenerate_view_matrix();
}

void Player::update(GLFWwindow *window, float delta_time)
{
    // Handle Input
    // Get movement direction
    glm::vec3 movement_direction = glm::vec3(0.0f, 0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        movement_direction += campera_front * movement_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        movement_direction -= campera_front * movement_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        movement_direction -= camera_right * movement_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movement_direction += camera_right * movement_speed;
    }

    if (movement_direction != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        // Normalize direction
        movement_direction = glm::normalize(movement_direction);

        // Apply the movement
        camera_pos += movement_direction * movement_speed * delta_time;
        
        regenerate_view_matrix();
    }
}

void Player::regenerate_view_matrix()
{
    view_matrix = glm::lookAt(camera_pos, camera_pos + campera_front, camera_up);
}

