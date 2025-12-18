#ifndef __ZOMBIE_H
#define __ZOMBIE_H

#include "object.h"
#include "actor.h"
#include <vector>
#include <string>

extern std::vector<std::string> zombie_move_files;
extern std::vector<std::string> zombie_die_files;
extern std::vector<std::string> zombie_eat_files;
void initZombieFilePath();

#define ZOMBIE_SPEED 10.0f

class Zombie : public Actor
{
    int frame_index_ = 0;
    int frame_count_ = 0;
    float frame_timer_ = 0.0f;
    float frame_delay_ = 0.1f;
    int fps_ = 10;
    float speed_ = ZOMBIE_SPEED;
    bool is_eating_ = false;
    int damage_ = 10;
public:
    static Zombie* addZombieChild(Object* parent, glm::vec2 pos, glm::ivec2 coor, float speed = 10.0f);

    virtual void init() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    void move(float dt);
    void eat(float dt);

    void takeDamage(int damage) override;
    void die(float dt) override;
    void hurt();

    // getters and setters
    int getDamage() const { return damage_; }
    void setDamage(int damage) { damage_ = damage; }
    bool getIsEating() const { return is_eating_; }
    void setIsEating(bool is_eating) {
        if (is_eating) {
            is_eating_ = true;
            speed_ = 0.0f;
            frame_count_ = static_cast<int>(zombie_eat_files.size());
        }else{
            is_eating_ = false;
            speed_ = ZOMBIE_SPEED;
            frame_count_ = static_cast<int>(zombie_move_files.size());
        }
        frame_timer_ = 0;
        frame_index_ = 0;
    }
    SDL_Texture* getTexture() const { return texture_; }
    void setTexture(SDL_Texture* texture) { texture_ = texture; }
    int getFrameIndex() const { return frame_index_; }
    void setFrameIndex(int frame_index) { frame_index_ = frame_index; }
    int getFrameCount() const { return frame_count_; }
    void setFrameCount(int frame_count) { frame_count_ = frame_count; }
    float getFrameTimer() const { return frame_timer_; }
    void setFrameTimer(float frame_timer) { frame_timer_ = frame_timer; }
    float getFrameDelay() const { return frame_delay_; }
    void setFrameDelay(float frame_delay) { frame_delay_ = frame_delay; }
    int getFps() const { return fps_; }
    void setFps(int fps) { fps_ = fps; }
    int getWidth() const { return width_; }
    void setWidth(int width) { width_ = width; }
    int getHeight() const { return height_; }
    void setHeight(int height) { height_ = height; }
    float getSpeed() const { return speed_; }
    void setSpeed(float speed) { speed_ = speed; }
};



#endif /* __ZOMBIE_H */