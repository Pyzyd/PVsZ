#include "object.h"

#include <fstream>

void Object::init()
{
}

void Object::handleEvents(SDL_Event &event)
{
    for (auto &child : children_)
    {
        if (child->getActive())
        {
            child->handleEvents(event);
        }
    }
}

void Object::update(float dt)
{
    for (auto it = children_.begin(); it != children_.end();)
    {
        auto child = *it;
        if (child->getNeedRemove())
        {
            it = children_.erase(it);
            child->clean();
            // delete child;
            child = nullptr;
        }
        else
        {
            if (child->getActive())
            {
                child->update(dt);
            }
            ++it;
        }
    }
}

void Object::render()
{
    for (auto &child : children_)
    {
        if (child->getActive())
        {
            child->render();
        }
    }
}

void Object::clean()
{
    for (auto &child : children_)
    {
        child->clean();
    }
    children_.clear();
}

void Object::sortChildren()
{
    children_.sort([](std::shared_ptr<Object> a, std::shared_ptr<Object> b){ return a->getObjectType() < b->getObjectType(); });
}

bool fileExists(std::string path)
{
    std::ifstream f(path.c_str());
    if (!f.good())
    {
        return false;
    }
    f.close();
    return true;
}
