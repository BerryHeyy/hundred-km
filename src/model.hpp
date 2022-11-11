#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

#include <OBJ_Loader.h>

#include "mesh.hpp"
#include "image_registry.hpp"

class Model
{
public:
    Model(const char *file_name);

    void draw(Shader &shader) const;

private:
    static const std::string RESOURCES_PATH;
    static const std::string TEXTURES_PATH;
    static const std::string OBJ_PATH;
    static const std::string MTL_PATH;

    std::vector<Mesh> meshes;

    void load_model(const char *file_name);
};
