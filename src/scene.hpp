#pragma once

#include <vector>

#include <vec3.hpp>

#include "spatial.hpp"
#include "model.hpp"

class Scene : public Spatial
{
public:
    std::vector<Model *> child_models;
    std::vector<Scene *> child_scenes;

    void add_scene(Scene* child_scene);
    void add_model(Model* child_model);

    void draw_scene() const;
};
