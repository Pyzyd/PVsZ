#ifndef __ACTOR_H
#define __ACTOR_H

#include "object.h"

class Actor : public Object
{
protected:
    int health_ = 0;
    glm::ivec2 coor_ = glm::ivec2(0,0);
    glm::vec2 pos_ = glm::vec2(0,0);
    bool alive_ = true;

public:
    virtual void init() override;
    virtual void update(float dt) override;
    virtual void clean() override;

    bool isAlive() const { return alive_; }
    void setAlive(bool alive) { alive_ = alive; }
    glm::ivec2 getCoor() const { return coor_; }
    void setCoor(glm::ivec2 coor) { coor_ = coor; }
    glm::vec2 getPos() const { return pos_; }
    void setPos(glm::vec2 pos) { pos_ = pos; }

    virtual void takeDamage(int damage) = 0;
    virtual void die(float dt) = 0;
};

#endif /* __ACTOR_H */
