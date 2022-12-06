#pragma once

#include <vector>

#include <vec3.hpp>
#include <mat4x4.hpp>

struct Transform
{
    const Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 transformation_matrix;

    void regenerate_transformation_matrix();  

    Transform operator + (const Transform& t) const;
};
