#include "scene_start.h"
#include "scene_main.h"

#include <SDL_image.h>


void SceneStart::init()
{
    Scene::init();
    background_ = IMG_LoadTexture(game_.getRenderer(), "res/menu.png");
    if (!background_) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    menu1_ = IMG_LoadTexture(game_.getRenderer(), "res/menu1.png");
    if (!menu1_) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    menu2_ = IMG_LoadTexture(game_.getRenderer(), "res/menu2.png");
    if (!menu2_) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }

}

void SceneStart::handleEvents(SDL_Event &event)
{
    Scene::handleEvents(event);
    menuClicked(event);
}

void SceneStart::update(float dt)
{
    Scene::update(dt);
}

void SceneStart::render()
{
    game_.renderBackground(background_, BG_START_START_X, BG_START_START_Y);
    renderMenu(menu_active_ ? menu2_ : menu1_, MENU1_START_X, MENU1_START_Y);
    Scene::render();
}

void SceneStart::clean()
{
    Scene::clean();
    if (background_) {
        SDL_DestroyTexture(background_);
    }
    if (menu1_) {
        SDL_DestroyTexture(menu1_);
    }
    if (menu2_) {
        SDL_DestroyTexture(menu2_);
    }
}

void SceneStart::renderMenu(SDL_Texture *menu, int x, int y)
{
    SDL_Rect dstRect = {x, y, 0, 0};
    SDL_QueryTexture(menu, NULL, NULL, &dstRect.w, &dstRect.h);
    SDL_RenderCopy(game_.getRenderer(), menu, NULL, &dstRect);
}

void SceneStart::menuClicked(SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        if (event.button.x > MENU1_START_X && event.button.x < MENU1_START_X + MENU1_WIDTH &&
            event.button.y > MENU1_START_Y && event.button.y < MENU1_START_Y + MENU1_HEIGHT) {
            // menu1 clicked
            menu_active_ = true;
        }
    }else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && menu_active_) {
        menu_active_ = false;
        if (event.button.x > MENU1_START_X && event.button.x < MENU1_START_X + MENU1_WIDTH &&
            event.button.y > MENU1_START_Y && event.button.y < MENU1_START_Y + MENU1_HEIGHT) {
            // menu1 up
            auto scene_main = new SceneMain();
            game_.changeScene(scene_main);
        }
    }
}
