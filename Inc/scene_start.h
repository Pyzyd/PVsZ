#ifndef __SCENE_START_H
#define __SCENE_START_H

#include "scene.h"

#define BG_START_START_X 0
#define BG_START_START_Y 0

#define MENU1_START_X 474
#define MENU1_START_Y 75

#define MENU1_WIDTH 300
#define MENU1_HEIGHT 140

#define MENU2_START_X 0
#define MENU2_START_Y 0

class SceneStart : public Scene
{
    SDL_Texture* menu1_ = nullptr;
    SDL_Texture* menu2_ = nullptr;
    bool menu_active_ = false;
public:
    virtual void init() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    void renderMenu(SDL_Texture* menu, int x, int y);
    void menuClicked(SDL_Event& event);
};


#endif /* __SCENE_START_H */
