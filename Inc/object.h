#ifndef __OBJECT_H
#define __OBJECT_H

#include "game.h"
#include <vector>
#include <list>
#include <string>


bool fileExists(std::string path);

enum class ObjectType {
    CARD,
    PLANT,
    SUNSHINE,
    NONE = -1,
};

class Object
{
protected:
    Game& game_ = Game::getInstance();
    std::list<Object*> children_;
    ObjectType o_type_ = ObjectType::NONE;

    bool need_remove_ = false; 
    bool is_active_ = true;
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void init();
    virtual void handleEvents(SDL_Event& event);
    virtual void update(float dt);
    virtual void render();
    virtual void clean();

    void setObjectType(ObjectType type) { o_type_ = type; }
    ObjectType getObjectType() { return o_type_; }

    bool getNeedRemove() const { return need_remove_; }
    void setNeedRemove(bool need_remove) { need_remove_ = need_remove; }
    void setActive(bool active) { is_active_ = active; }
    bool getActive() const { return is_active_; }

    virtual void addChild(Object* child) {
        children_.push_back(child);
        sortChildren();
    }
    virtual void removeChild(Object* child) {
        children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
    }

    void sortChildren();
};


#endif /* __OBJECT_H */
