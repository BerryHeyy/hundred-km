#include "spatial.hpp"

Spatial::Spatial()
{
    transform.regenerate_transformation_matrix();
}

bool Spatial::has_parent() const
{
    return transform.parent != nullptr;
}

/* #region getters and setters */

void Spatial::set_transform(const Transform& transform)
{
    this->transform = transform;
    this->transform.regenerate_transformation_matrix();
}

void Spatial::set_parent(const Transform* parent)
{
    transform.parent = parent;
    transform.regenerate_transformation_matrix();
}

void Spatial::set_scale(const glm::vec3 &scale)
{
    transform.scale = scale;
    transform.regenerate_transformation_matrix();
}

void Spatial::set_rotation(const glm::vec3 &rotation)
{
    transform.rotation = rotation;
    transform.regenerate_transformation_matrix();
}

void Spatial::set_position(const glm::vec3 &position)
{
    transform.position = position;
    transform.regenerate_transformation_matrix();
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


Transform& Spatial::get_transform()
{
    return transform;
}

const Transform* Spatial::get_parent() const
{
    return transform.parent;
}

const glm::vec3& Spatial::get_scale() const
{
    return transform.scale;
}

const glm::vec3& Spatial::get_rotation() const
{
    return transform.rotation;
}

const glm::vec3& Spatial::get_position() const
{
    return transform.position;
}

const glm::mat4& Spatial::get_transformation_matrix() const
{
    return transform.transformation_matrix;
}

/* #endregion */
