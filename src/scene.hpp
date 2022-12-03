#pragma once

#include <vector>

#include <vec3.hpp>

#include "spatial.hpp"
#include "model.hpp"

class Scene : Spatial
{
public:
    std::vector<Model> models;
    std::vector<Scene> children;

    void add_child(const Scene& child);
    void add_model(const Model& model);

    void draw_scene() const;
};
