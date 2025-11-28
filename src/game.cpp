#include "game.h"
#include "scene_main.h"

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
    is_running_ = true;
    dt_ = 0.0f;
    frame_delay_ = 1000.0f / FPS_;

    // 创建场景
    current_scene_ = new SceneMain();
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
    current_scene_->update(dt);
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
        delete current_scene_;
        current_scene_ = nullptr;
    }

    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
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

void Game::renderBackground(SDL_Texture *texture)
{
    if (texture)
    {
        SDL_Rect src_rect = {WINDOW_START_X, WINDOW_START_Y, static_cast<int>(screen_size_.x), static_cast<int>(screen_size_.y)};
        // SDL_FRect dst_rect = {0, 0, screen_size_.x, screen_size_.y};
        // SDL_QueryTexture(texture, nullptr, nullptr, &src_rect.w, &src_rect.h);
        SDL_RenderCopyF(renderer_, texture, &src_rect, nullptr);
    }
}
