#include "game.h"
#include "scene_main.h"
#include "scene_start.h"
#include "asset_store.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

void Game::run()
{
    while (is_running_)
    {
        auto start = SDL_GetTicks64();
        handleEvents();
        update(dt_);
        render();
        auto end = SDL_GetTicks64();
        auto elapsed = end - start;
        if (elapsed < frame_delay_)
        {
            SDL_Delay(frame_delay_ - elapsed);
            dt_ = frame_delay_ / 1000.0f;
        }
        else
        {
            dt_ = elapsed / 1000.0f;
        }
        // SDL_Log("FPS: %f\n", 1.0f / dt_);
    }
}

void Game::init(std::string title, int width, int height)
{
    gen = std::mt19937(rd());

    screen_size_ = glm::vec2(width, height);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return;
    }
    if (TTF_Init() != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }
    Mix_AllocateChannels(32);

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window_, &renderer_);
    if (window_ == nullptr || renderer_ == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindowAndRenderer Error: %s\n", SDL_GetError());
        return;
    }
    SDL_SetWindowTitle(window_, title.c_str());
    SDL_RenderSetLogicalSize(renderer_, width, height);
    SDL_RenderSetIntegerScale(renderer_, SDL_FALSE);  // 允许非整数缩放

    asset_store_ = std::make_shared<AssetStore>(renderer_);

    is_running_ = true;
    dt_ = 0.0f;
    frame_delay_ = 1000.0f / FPS_;

    // 创建场景
    current_scene_ = std::make_shared<SceneStart>();
    current_scene_->init();
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            is_running_ = false;
            break;
        default:
            current_scene_->handleEvents(event);
        }
    }
}

void Game::update(float dt)
{
    moveMouse();
    if (current_scene_)
    {
        current_scene_->update(dt);
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
    // Draw here
    current_scene_->render();
    SDL_RenderPresent(renderer_);
}

void Game::clean()
{
    if (current_scene_)
    {
        current_scene_->clean();
        // delete current_scene_;
        current_scene_ = nullptr;
    }

    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (asset_store_){
        asset_store_->clean();
        // delete asset_store_;
        asset_store_ = nullptr;
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::renderBackground(SDL_Texture* texture, int start_x, int start_y)
{
    if (texture)
    {
        SDL_FRect dst_rect = {start_x, start_y, 0, 0};
        int w,h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        dst_rect.w = w;
        dst_rect.h = h;
        SDL_RenderCopyF(renderer_, texture, nullptr, &dst_rect);
    }
}

void Game::moveMouse()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    setMousePos(glm::vec2(x, y));
}

void Game::changeScene(std::shared_ptr<Scene> scene)
{
    if (current_scene_ != nullptr)
    {
        current_scene_->clean();
        // delete current_scene_;
        current_scene_ = nullptr;
    }
    current_scene_ = scene;
    current_scene_->init();
}
