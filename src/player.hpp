#pragma once

#include <GLFW/glfw3.h>
#include <vec3.hpp>
#include <mat4x4.hpp>

class Player
{
public:
    glm::vec3 camera_pos;
    glm::vec3 campera_front;
    glm::vec3 camera_up;
    glm::vec3 camera_right;

    glm::mat4 view_matrix;

    float movement_speed;
    
    Player();

    void update(GLFWwindow *window);

    void regenerate_view_matrix();


};
