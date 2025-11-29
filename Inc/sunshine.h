#ifndef __SUNSHINE_H
#define __SUNSHINE_H

#include "object.h"

extern std::vector<std::string> sunshine_file_path;

void initSunshineFilePath();

class SunShine : public Object
{
    glm::vec2 pos_ = glm::vec2(0, 0);
    glm::vec2 dest_ = glm::vec2(0, 0);
    SDL_Texture* texture_ = nullptr;
    int frame_index_ = 0;
    int frame_count_ = 0;
    float frame_time_ = 0.0f;
    int fps_ = 40;
    float speed_ = 20.0f;
public:
    static SunShine* addSunshineChild(Object* parent, glm::vec2 pos, glm::vec2 dest);

    virtual void init() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    // getters and setters
    glm::vec2 getPos() const { return pos_; }
    void setPos(glm::vec2 pos) { pos_ = pos; }
    glm::vec2 getDest() const { return dest_; }
    void setDest(glm::vec2 dest) { dest_ = dest; }
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
};

#endif /* __SUNSHINE_H */
