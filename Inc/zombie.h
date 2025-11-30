#ifndef __ZOMBIE_H
#define __ZOMBIE_H

#include "object.h"
#include <vector>
#include <string>

extern std::vector<std::string> zombie_file_path;

void initZombieFilePath();

class Zombie : public Object
{
    glm::vec2 pos_ = glm::vec2(0,0);
    SDL_Texture* texture_ = nullptr;
    int frame_index_ = 0;
    int frame_count_ = 0;
    float frame_timer_ = 0.0f;
    float frame_delay_ = 0.1f;
    int fps_ = 10;
    int width_ = 0;
    int height_ = 0;
    float speed_ = 3.0f;
public:
    static Zombie* addZombieChild(Object* parent, glm::vec2 pos, float speed = 10.0f);

    virtual void init() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    // getters and setters
    glm::vec2 getPos() const { return pos_; }
    void setPos(glm::vec2 pos) { pos_ = pos; }
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