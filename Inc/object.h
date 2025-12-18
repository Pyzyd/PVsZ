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
    PLANT_BULLET,
    ZOMBIE,
    SUNSHINE,
    NONE = -1,
};

class Object : public std::enable_shared_from_this<Object>
{
protected:
    Game& game_ = Game::getInstance();
    std::list<std::shared_ptr<Object>> children_;
    ObjectType o_type_ = ObjectType::NONE;
    std::shared_ptr<Object> parent_ = nullptr;
    SDL_Texture* texture_ = nullptr;
    int width_ = 0;
    int height_ = 0;

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

    std::shared_ptr<Object> getParent() { return parent_; }
    void setParent(std::shared_ptr<Object> parent) { parent_ = parent; }

    void setObjectType(ObjectType type) { o_type_ = type; }
    ObjectType getObjectType() { return o_type_; }

    bool getNeedRemove() const { return need_remove_; }
    void setNeedRemove(bool need_remove) { need_remove_ = need_remove; }
    void setActive(bool active) { is_active_ = active; }
    bool getActive() const { return is_active_; }

    std::list<std::shared_ptr<Object>>& getChildren() { return children_; }

    virtual void addChild(std::shared_ptr<Object> child) {
        children_.push_back(child);
        sortChildren();
    }
    virtual void removeChild(std::shared_ptr<Object> child) {
        children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
    }

    void sortChildren();
};


#endif /* __OBJECT_H */
