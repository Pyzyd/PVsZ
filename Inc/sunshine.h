#ifndef __SUNSHINE_H
#define __SUNSHINE_H

#include "object.h"
#include <SDL_mixer.h>

#define SUNSHINE_CLICKED_SOUND "res/sunshine.mp3"

extern std::vector<std::string> sunshine_file_path;

void initSunshineFilePath();

class SunShine : public Object
{
    glm::vec2 pos_ = glm::vec2(0, 0);
    glm::vec2 dest_ = glm::vec2(0, 0);
    glm::vec2 direction_ = glm::vec2(0, 0);
    int width_ = 0;
    int height_ = 0;
    SDL_Texture* texture_ = nullptr;
    int frame_index_ = 0;
    int frame_count_ = 0;
    float frame_time_ = 0.0f;
    int fps_ = 60;
    float speed_ = 20.0f;
    bool clicked_ = false;
    bool is_collected_ = false;
    bool has_sound_ = false;
    Mix_Chunk* clicked_sound_ = nullptr;

public:
    static int value;
    static std::shared_ptr<SunShine> addSunshineChild(std::shared_ptr<Object> parent, glm::vec2 pos, glm::vec2 dest);

    virtual void init() override;
    virtual void handleEvents(SDL_Event &event) override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    // getters and setters
    bool isCollected() const { return is_collected_; }
    void setCollected(bool is_collected) { is_collected_ = is_collected; }
    bool isClicked() const { return clicked_; }
    void setClicked(bool clicked) { clicked_ = clicked; }
    glm::vec2 getDirection() const { return direction_; }
    void setDirection(glm::vec2 direction) { direction_ = direction; }
    glm::vec2 getPos() const { return pos_; }
    void setPos(glm::vec2 pos)
    {
        pos_ = pos;
    }
    glm::vec2 getDest() const { return dest_; }
    void setDest(glm::vec2 dest)
    {
        dest_ = dest;
        direction_ = glm::normalize(dest - pos_);
    }
    SDL_Texture* getTexture() const { return texture_; }
    void setTexture(SDL_Texture* texture) { texture_ = texture; }
    int getFrameIndex() const { return frame_index_; }
    void setFrameIndex(int frame_index) { frame_index_ = frame_index; }
    int getFrameCount() const { return frame_count_; }
    void setFrameCount(int frame_count) { frame_count_ = frame_count; }
    float getFrameTime() const { return frame_time_; }
    void setFrameTime(float frame_time) { frame_time_ = frame_time; }
    int getFps() const { return fps_; }
    void setFps(int fps) { fps_ = fps; }
    float getSpeed() const { return speed_; }
    void setSpeed(float speed) { speed_ = speed; }
    void clicked(SDL_Event &event);
};

#endif /* __SUNSHINE_H */
