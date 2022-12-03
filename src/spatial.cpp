#include "spatial.hpp"

#include <gtc/matrix_transform.hpp>

Spatial::Spatial()
{
    scale = glm::vec3(1, 1, 1);
    rotation = glm::vec3(0, 0, 0);
    position = glm::vec3(0, 0, 0);

    regenerate_transformation_matrix();
}

/* #region getters and setters */

void Spatial::set_scale(const glm::vec3 &scale)
{
    this->scale = scale;
    regenerate_transformation_matrix();
}

void Spatial::set_rotation(const glm::vec3 &rotation)
{
    this->rotation = rotation;
    regenerate_transformation_matrix();
}

void Spatial::set_position(const glm::vec3 &position)
{
    this->position = position;
    regenerate_transformation_matrix();
}

void Spatial::set_scale(float x, float y, float z)
{
    set_scale(glm::vec3(x, y, z));
}

void Spatial::set_rotation(float x, float y, float z)
{
    set_rotation(glm::vec3(x, y, z));
}

void Spatial::set_position(float x, float y, float z)
{
    set_position(glm::vec3(x, y, z));
}


const glm::vec3& Spatial::get_scale() const
{
    return scale;
}

const glm::vec3& Spatial::get_rotation() const
{
    return rotation;
}

const glm::vec3& Spatial::get_position() const
{
    return position;
}

const glm::mat4& Spatial::get_transformation_matric() const
{
    return transformation_matrix;
}

/* #endregion */

void Spatial::regenerate_transformation_matrix()
{
    transformation_matrix = glm::mat4(1.0f);

    transformation_matrix = glm::scale(transformation_matrix, scale);

    transformation_matrix = glm::rotate(transformation_matrix, rotation.x, glm::vec3(1.0, 0.0, 0.0));
    transformation_matrix = glm::rotate(transformation_matrix, rotation.y, glm::vec3(0.0, 1.0, 0.0));
    transformation_matrix = glm::rotate(transformation_matrix, rotation.z, glm::vec3(0.0, 0.0, 1.0));

    transformation_matrix = glm::translate(transformation_matrix, position);
}
