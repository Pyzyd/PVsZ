#ifndef __SCENE_H
#define __SCENE_H

#include "object.h"
#include <glm/glm.hpp>
#include <vector>

class Scene : public Object
{
protected:
    SDL_Texture* background_;
    std::vector<Object*> objects_;
public:
    Scene() = default;
    virtual ~Scene() = default;
    virtual void init() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;
};


#endif /* __SCENE_H */
