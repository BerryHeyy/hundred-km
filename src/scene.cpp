#include "scene.hpp"

void Scene::add_child(const Scene& child)
{
    children.push_back(child);
}

void Scene::add_model(const Model& model)
{
    models.push_back(model);
}

void Scene::draw_scene() const
{
    for (int i = 0; i < children.size(); i++)
    {
        children[i].draw_scene();
    }

    for (int i = 0; i < models.size(); i++)
    {
        models[i].draw();
    }
}
