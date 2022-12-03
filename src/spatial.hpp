#pragma once

#include <vec3.hpp>
#include <mat4x4.hpp>

class Spatial
{
protected:
    Spatial();

public:
    glm::vec3 scale, rotation, position;
    glm::mat4 transformation_matrix;

    void set_scale(const glm::vec3 &scale);
    void set_rotation(const glm::vec3 &rotation);
    void set_position(const glm::vec3 &position);
    void set_scale(float x, float y, float z);
    void set_rotation(float x, float y, float z);
    void set_position(float x, float y, float z);

    const glm::vec3& get_scale() const;
    const glm::vec3& get_rotation() const;
    const glm::vec3& get_position() const;

    const glm::mat4& get_transformation_matric() const;

private:
    void regenerate_transformation_matrix();    
};
