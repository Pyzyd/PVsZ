#ifndef __SCENE_MAIN_H
#define __SCENE_MAIN_H

#include "scene.h"
#include <array>
#include <SDL_ttf.h>

#define BG_MAIN_START_X -100
#define BG_START_Y 0

#define TOP_BAR_START_X (250 + BG_MAIN_START_X)
#define TOP_BAR_START_Y (0 + BG_START_Y)
#define TOP_BAR_CARD_NUM 8

#define TOP_BAR_CARD_START_X (338 + BG_MAIN_START_X)
#define TOP_BAR_CARD_START_Y (6.0 + BG_START_Y)
#define TOP_BAR_CARD_INTERVAL  62.5f
#define TOP_BAR_CARD_HEIGHT 96.0f

#define PLANT_MAP_START_X (256 + BG_MAIN_START_X)
#define PLANT_MAP_START_Y (179 + BG_START_Y)
#define PLANT_MAP_GRID_H  102
#define PLANT_MAP_GRID_W  81

#define PLANT_MAP_GRID_ROWS 3
#define PLANT_MAP_GRID_COLS 9

#define PLANT_MAP_WIDTH (PLANT_MAP_GRID_COLS * PLANT_MAP_GRID_W)
#define PLANT_MAP_HEIGHT (PLANT_MAP_GRID_ROWS * PLANT_MAP_GRID_H)

#define RANDOM_SUNSHINE_NUMS 10

#define SUNSHINE_NUM_TEXT_START_X (276 + BG_MAIN_START_X)
#define SUNSHINE_NUM_TEXT_START_Y (67 + BG_START_Y)
#define SUNSHINE_NUM_TEXT_CENTER_X (SUNSHINE_NUM_TEXT_START_X + 20)
#define SUNSHINE_NUM_TEXT_CENTER_Y (SUNSHINE_NUM_TEXT_START_Y + 16)

#define ZOMBIE_NUM_MAX 10

class Plant;
class Zombie;
class SunShine;
class Bullet;

class SceneMain : public Scene
{
private:
    SDL_Texture* top_bar_;
    bool card_clicked_ = false;
    int card_num_ = 0;
    Plant* clicked_card_plant_ = nullptr;
    float sunshine_timer_ = 0.0f;
    float sunshine_interval_ = 5.0f;
    int sunshine_num_ = 0;
    int total_sunshine_num_ = 0;
    std::array<std::array<bool, PLANT_MAP_GRID_COLS>, PLANT_MAP_GRID_ROWS> plant_map_;
    glm::vec2 sunshine_collector_pos_;
    std::vector<bool> has_zombie_;

    TTF_Font* font_ = nullptr;
    float zombie_timer_ = 0.0f;
    float zombie_interval_ = 8.0f;

    int zombie_num_ = 0;
public:
    SceneMain() = default;
    virtual ~SceneMain() = default;

    virtual void init() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    void renderTopBar();
    void renderSunShineNum();

    void updatePlant();

    void userClickedCard(SDL_Event &event);
    void setClickedCardPlant(int index);
    void createRandomSunShine(float dt);
    void countTotalSunShine();
    void createZombie(float dt);
    void plantAttackZombie();
    void ZombieEmerge();
    void ZombieTakeDamage(Bullet* bullet);
    void ZombieEatPlant();
    Plant* getPlantFromMapCoor(glm::ivec2 map_coor);
    Zombie* getZombieFromMapCoor(glm::ivec2 map_coor);


    glm::vec2 getSunshineCollectorPos() { return sunshine_collector_pos_; }
    static glm::ivec2 posToMapCoor(glm::vec2 pos);
};



#endif /* __SCENE_MAIN_H */
