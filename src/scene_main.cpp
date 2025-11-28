#include "scene_main.h"
#include "plant.h"

#include <SDL_image.h>

void SceneMain::init()
{
    Scene::init();
    background_ = IMG_LoadTexture(game_.getRenderer(), "res/bg.jpg");
    if (!background_) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    top_bar_ = IMG_LoadTexture(game_.getRenderer(), "res/bar5.png");
    if (!top_bar_) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    initFilePath(); // 初始化文件路径
    for (int i = 0; i < (TOP_BAR_CARD_NUM > static_cast<int>(PLANT_TYPE::COUNT) ? static_cast<int>(PLANT_TYPE::COUNT) : TOP_BAR_CARD_NUM); ++i) {
        Card::addCardChild(this, static_cast<PLANT_TYPE>(i), glm::vec2(TOP_BAR_CARD_START_X + i * TOP_BAR_CARD_INTERVAL, TOP_BAR_CARD_START_Y));
    }
}

void SceneMain::handleEvents(SDL_Event &event)
{
    Scene::handleEvents(event);
}

void SceneMain::update(float dt)
{
    Scene::update(dt);
}

void SceneMain::render()
{
    game_.renderBackground(background_);
    renderTopBar();
    Scene::render();
}

void SceneMain::clean()
{
    Scene::clean();
    if (!background_) {
        SDL_DestroyTexture(background_);
        background_ = nullptr;
    }
}

void SceneMain::renderTopBar()
{
    if (top_bar_) {
        SDL_FRect dest = {TOP_BAR_START_X, TOP_BAR_START_Y, 800, 100};
        int w, h;
        SDL_QueryTexture(top_bar_, nullptr, nullptr, &w, &h);
        dest.w = w;
        dest.h = h;
        SDL_RenderCopyF(game_.getRenderer(), top_bar_, nullptr, &dest);
    }
}
