#include "model.hpp"

#include <stdexcept>
#include <string>

#define OBJL_CONSOLE_OUTPUT
#include <OBJ_Loader.h>
#include <gtc/matrix_transform.hpp>

#include "path_helper.hpp"
#include "image_registry.hpp"

Model::Model(const char *file_name)
{
    scale = glm::vec3(1, 1, 1);
    rotation = glm::vec3(0, 0, 0);
    position = glm::vec3(0, 0, 0);

    regenerate_transformation_matrix();

    load_model(file_name);
}

void Model::draw(Shader &shader) const
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        shader.set_mat4("model_transform", transformation_matrix);
        meshes[i].draw(shader);
    }
}

/* #region getters and setters */

void Model::set_scale(const glm::vec3 &scale)
{
    this->scale = scale;
    regenerate_transformation_matrix();
}

void Model::set_rotation(const glm::vec3 &rotation)
{
    this->rotation = rotation;
    regenerate_transformation_matrix();
}

void Model::set_position(const glm::vec3 &position)
{
    this->position = position;
    regenerate_transformation_matrix();
}

void Model::set_scale(float x, float y, float z)
{
    set_scale(glm::vec3(x, y, z));
}

void Model::set_rotation(float x, float y, float z)
{
    set_rotation(glm::vec3(x, y, z));
}

void Model::set_position(float x, float y, float z)
{
    set_position(glm::vec3(x, y, z));
}


const glm::vec3& Model::get_scale() const
{
    return scale;
}

const glm::vec3& Model::get_rotation() const
{
    return rotation;
}

const glm::vec3& Model::get_position() const
{
    return position;
}

const glm::mat4& Model::get_transformation_matric() const
{
    return transformation_matrix;
}

/* #endregion */

void Model::regenerate_transformation_matrix()
{
    transformation_matrix = glm::mat4(1.0f);

    transformation_matrix = glm::scale(transformation_matrix, scale);

    transformation_matrix = glm::rotate(transformation_matrix, rotation.x, glm::vec3(1.0, 0.0, 0.0));
    transformation_matrix = glm::rotate(transformation_matrix, rotation.y, glm::vec3(0.0, 1.0, 0.0));
    transformation_matrix = glm::rotate(transformation_matrix, rotation.z, glm::vec3(0.0, 0.0, 1.0));

    transformation_matrix = glm::translate(transformation_matrix, position);
}

void Model::load_model(const char *file_name)
{
    objl::Loader loader;

    std::string pa = OBJ_PATH + file_name;

    if (!loader.LoadFile(OBJ_PATH + file_name))
    {
        
        throw std::runtime_error("Failed to load obj file `" + std::string(file_name) + "`.");
    }

    for (objl::Mesh m : loader.LoadedMeshes)
    {
        Mesh mesh;
        mesh.vertices.resize(m.Vertices.size());
        mesh.indices.resize(m.Indices.size());

        for (int i = 0; i < m.Vertices.size(); i++)
        {
            glm::vec3 pos(m.Vertices[i].Position.X,
                m.Vertices[i].Position.Y,
                m.Vertices[i].Position.Z);

            glm::vec3 normal(m.Vertices[i].Normal.X,
                m.Vertices[i].Normal.Y,
                m.Vertices[i].Normal.Z);

            glm::vec2 tex_coords(m.Vertices[i].TextureCoordinate.X,
                m.Vertices[i].TextureCoordinate.Y);

            mesh.vertices[i] = Vertex { pos, normal, tex_coords };
        }

        mesh.indices = m.Indices;

        mesh.texture = image_registry::get_or_load_texture(m.MeshMaterial.map_Kd);

        mesh.initialize_mesh();

        meshes.push_back(mesh);
    }
}


