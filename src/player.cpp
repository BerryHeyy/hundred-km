#include "player.hpp"

#include <gtc/matrix_transform.hpp>

// Camera variables
static glm::vec3 camera_pos;
static glm::vec3 camera_rot;
static glm::vec3 campera_front;
static glm::vec3 camera_up;

static glm::mat4 view_matrix;

static float movement_speed;
static float camera_sensitivity;

static float camera_last_x, camera_last_y;

void player::init(int swidth, int sheight)
{
    camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    camera_rot = glm::vec3(0.0f, -90.0f, 0.0f);
    campera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    movement_speed = 2.0f;
    camera_sensitivity = 0.1f;

    camera_last_x = swidth / 2;
    camera_last_y = sheight / 2;

    regenerate_view_matrix();
}

const glm::mat4& player::get_view_matrix()
{
    return view_matrix;
}

void player::handle_movement(GLFWwindow *window, float delta_time)
{
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
        movement_direction -= glm::normalize(glm::cross(campera_front, camera_up)) * movement_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movement_direction += glm::normalize(glm::cross(campera_front, camera_up)) * movement_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        movement_direction += glm::normalize(glm::cross(glm::cross(campera_front, camera_up), campera_front)) * movement_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        movement_direction -= glm::normalize(glm::cross(glm::cross(campera_front, camera_up), campera_front)) * movement_speed;
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

void player::handle_mouse(GLFWwindow* window, double xpos, double ypos)
{
    static bool first_mouse = true;

    if (first_mouse)
    {
        camera_last_x = xpos;
        camera_last_y = ypos;
        first_mouse = false;
    }

    float x_offset = xpos - camera_last_x;
    float y_offset = ypos - camera_last_y;
    camera_last_x = xpos;
    camera_last_y = ypos;
    
    y_offset *= camera_sensitivity;
    x_offset *= camera_sensitivity;
    
    camera_rot.x -= y_offset;
    camera_rot.y = glm::mod(camera_rot.y + x_offset, (GLfloat) 360.0f);;

    if (camera_rot.x > 89.0f)
    {
        camera_rot.x = 89.0f;
    }
    if (camera_rot.x < -89.0f)
    {
        camera_rot.x = -89.0f;
    }

    glm::vec3 camera_direction;
    camera_direction.x = cos(glm::radians(camera_rot.y)) * cos(glm::radians(camera_rot.x));
    camera_direction.y = sin(glm::radians(camera_rot.x));
    camera_direction.z = sin(glm::radians(camera_rot.y)) * cos(glm::radians(camera_rot.x));

    campera_front = glm::normalize(camera_direction);
    regenerate_view_matrix();
}

void player::update(GLFWwindow *window, float delta_time)
{
    // Handle Input
    handle_movement(window, delta_time);
}

void player::regenerate_view_matrix()
{
    view_matrix = glm::lookAt(camera_pos, camera_pos + campera_front, camera_up);
}

