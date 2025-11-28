#include "scene.h"

void Scene::init()
{
    Object::init();
}

void Scene::handleEvents(SDL_Event &event)
{
    Object::handleEvents(event);
}

void Scene::update(float dt)
{
    Object::update(dt);
}

void Scene::render()
{
    Object::render();
}

void Scene::clean()
{
    Object::clean();
    if (!background_)
    {
        SDL_DestroyTexture(background_);
        background_ = nullptr;
    }
    if (!objects_.empty())
    {
        for (auto &object : objects_)
        {
            object->clean();
        }
        objects_.clear();
    }
}
