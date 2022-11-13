#pragma once

#include <vector>

#include "mesh.hpp"

class Model
{
public:
    Model(const char *file_name);

    void draw(Shader &shader) const;

private:
    

    std::vector<Mesh> meshes;

    void load_model(const char *file_name);
};
