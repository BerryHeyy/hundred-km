#include "transform.hpp"

#include <gtc/matrix_transform.hpp>

void Transform::regenerate_transformation_matrix()
{
    transformation_matrix = glm::mat4(1.0f);

    transformation_matrix = glm::scale(transformation_matrix, scale);

    transformation_matrix = glm::rotate(transformation_matrix, rotation.x, glm::vec3(1.0, 0.0, 0.0));
    transformation_matrix = glm::rotate(transformation_matrix, rotation.y, glm::vec3(0.0, 1.0, 0.0));
    transformation_matrix = glm::rotate(transformation_matrix, rotation.z, glm::vec3(0.0, 0.0, 1.0));

    transformation_matrix = glm::translate(transformation_matrix, position);

    if (parent != nullptr) 
    {
        transformation_matrix *= parent->transformation_matrix;
    }
    
    if (children.size() > 0)
    {
        for (size_t i = 0; i < children.size(); i++)
        {
            children[i]->regenerate_transformation_matrix();
        }
    }
}

Transform Transform::operator + (const Transform& t) const
{
    Transform ret;
    ret.scale = glm::vec3(0.0f, 0.0f, 0.0f);

    ret.position += t.position + position;
    ret.rotation += t.rotation + rotation;
    ret.scale += t.scale + scale;

    return ret;
}