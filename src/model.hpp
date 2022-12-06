#pragma once

#include <vector>

#include <vec3.hpp>
#include <mat4x4.hpp>

#include "spatial.hpp"
#include "mesh.hpp"

class Model : public Spatial
{
public:
    Model(const char *file_name, Shader* shader);

    void draw() const;
    void draw(const Transform& parent_transform) const;

private:
    Shader* shader;

    std::vector<Mesh> meshes;

    void load_model(const char *file_name);
};
