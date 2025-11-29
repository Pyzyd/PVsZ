#ifndef __ZOMBIE_H
#define __ZOMBIE_H

#include "object.h"

class Zombie : public Object
{
    glm::vec2 pos_ = glm::vec2(0,0);
    SDL_Texture* texture_ = nullptr;
public:

};



#endif /* __ZOMBIE_H */