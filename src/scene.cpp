#include "scene.hpp"

#include <stdexcept>

void Scene::add_scene(Scene* child_scene)
{
    if (child_scene->has_parent())
        throw std::runtime_error("Tried adding a scene that already has a parent to another scene.");

    child_scene->set_parent(&transform);
    transform.children.push_back(&child_scene->get_transform());

    child_scenes.push_back(child_scene);

    child_scene->get_transform().regenerate_transformation_matrix();
}

void Scene::add_model(Model* child_model)
{
    if (child_model->has_parent())
        throw std::runtime_error("Tried adding a model that already has a parent to a scene.");

    child_model->set_parent(&transform);
    transform.children.push_back(&child_model->get_transform());

    child_models.push_back(child_model);

    child_model->get_transform().regenerate_transformation_matrix();
}

void Scene::draw_scene() const
{
    for (int i = 0; i < child_scenes.size(); i++)
    {
        child_scenes[i]->draw_scene();
    }

    for (int i = 0; i < child_models.size(); i++)
    {
        child_models[i]->draw();
    }
}

