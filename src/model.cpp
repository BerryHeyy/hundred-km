#include "model.hpp"

const std::string Model::RESOURCES_PATH = ".\\resources\\";
const std::string Model::TEXTURES_PATH = Model::RESOURCES_PATH + "texture\\";
const std::string Model::OBJ_PATH = Model::RESOURCES_PATH + "model\\obj\\";
const std::string Model::MTL_PATH = Model::OBJ_PATH;

Model::Model(const char *file_name)
{
    load_model(file_name);
}

void Model::load_model(const char *file_name)
{
    objl::Loader loader;

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

void Model::draw(Shader &shader) const
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader);
    }
}
