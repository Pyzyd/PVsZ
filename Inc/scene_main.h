#ifndef __SCENE_MAIN_H
#define __SCENE_MAIN_H

#include "scene.h"

#define TOP_BAR_START_X (250 - WINDOW_START_X)
#define TOP_BAR_START_Y (0 - WINDOW_START_Y)
#define TOP_BAR_CARD_NUM 8

#define TOP_BAR_CARD_START_X (338 - WINDOW_START_X)
#define TOP_BAR_CARD_START_Y (6.0 - WINDOW_START_Y)
#define TOP_BAR_CARD_INTERVAL  62.5f


class SceneMain : public Scene
{
private:
    SDL_Texture* top_bar_;
public:
    SceneMain() = default;
    virtual ~SceneMain() = default;

    virtual void init() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    void renderTopBar();
};



#endif /* __SCENE_MAIN_H */
