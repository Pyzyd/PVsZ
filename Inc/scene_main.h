#ifndef __SCENE_MAIN_H
#define __SCENE_MAIN_H

#include "scene.h"
#include <array>

#define TOP_BAR_START_X (250 - WINDOW_START_X)
#define TOP_BAR_START_Y (0 - WINDOW_START_Y)
#define TOP_BAR_CARD_NUM 8

#define TOP_BAR_CARD_START_X (338 - WINDOW_START_X)
#define TOP_BAR_CARD_START_Y (6.0 - WINDOW_START_Y)
#define TOP_BAR_CARD_INTERVAL  62.5f
#define TOP_BAR_CARD_HEIGHT 96.0f

#define PLANT_MAP_START_X (256 - WINDOW_START_X)
#define PLANT_MAP_START_Y (179 - WINDOW_START_Y)
#define PLANT_MAP_GRID_H  102
#define PLANT_MAP_GRID_W  81

#define PLANT_MAP_GRID_ROWS 3
#define PLANT_MAP_GRID_COLS 9

#define PLANT_MAP_WIDTH (PLANT_MAP_GRID_COLS * PLANT_MAP_GRID_W)
#define PLANT_MAP_HEIGHT (PLANT_MAP_GRID_ROWS * PLANT_MAP_GRID_H)

class Plant;

class SceneMain : public Scene
{
private:
    SDL_Texture* top_bar_;
    bool card_clicked_ = false;
    int card_num_ = 0;
    Plant* clicked_card_plant_ = nullptr;
    std::array<std::array<bool, PLANT_MAP_GRID_COLS>, PLANT_MAP_GRID_ROWS> plant_map_;
public:
    SceneMain() = default;
    virtual ~SceneMain() = default;

    virtual void init() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    void renderTopBar();

    void userClickedCard(SDL_Event &event);
    void setClickedCardPlant(int index);
};



#endif /* __SCENE_MAIN_H */
