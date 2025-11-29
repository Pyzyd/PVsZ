#include "sunshine.h"
#include "scene_main.h"
#include <SDL_image.h>

std::vector<std::string> sunshine_file_path;

int SunShine::value = 25;

SunShine *SunShine::addSunshineChild(Object *parent, glm::vec2 pos, glm::vec2 dest)
{
    SunShine *sunshine = new SunShine();
    sunshine->setPos(pos);
    sunshine->setDest(dest);
    sunshine->init();
    if (parent)
    {
        parent->addChild(sunshine);
    }
    return sunshine;
}

void SunShine::init()
{
    Object::init();
    o_type_ = ObjectType::SUNSHINE;
    texture_ = IMG_LoadTexture(game_.getRenderer(), sunshine_file_path[frame_index_].c_str());
    if (!texture_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    SDL_QueryTexture(texture_, nullptr, nullptr, &width_, &height_);
    direction_ = glm::normalize(dest_ - pos_);
    clicked_sound_ = Mix_LoadWAV(SUNSHINE_CLICKED_SOUND);
}

void SunShine::handleEvents(SDL_Event &event)
{
    Object::handleEvents(event);
    clicked(event);
}

void SunShine::update(float dt)
{
    Object::update(dt);
    pos_ = pos_ + direction_ * speed_ * dt;

    if (glm::distance(pos_, dest_) < speed_ * dt)
    {
        pos_ = dest_;
        speed_ = 0.0;
        if (dest_ == glm::vec2((TOP_BAR_START_X + TOP_BAR_CARD_START_X) / 2.0f, TOP_BAR_CARD_HEIGHT / 2.0f))
        {
            is_collected_ = true;
        }
    }
    frame_time_ += dt;
    if (frame_time_ >= 1.0 / fps_)
    {
        frame_time_ = 0.0;
        frame_index_++;
        if (frame_index_ >= sunshine_file_path.size())
        {
            frame_index_ = 0;
            if (!clicked_ && is_collected_){
                setNeedRemove(true);
            }
        }
        texture_ = IMG_LoadTexture(game_.getRenderer(), sunshine_file_path[frame_index_].c_str());
        if (!texture_)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
        }
    }
}

void SunShine::render()
{
    Object::render();
    SDL_Rect rect = {static_cast<int>(pos_.x - width_ / 2.0f), static_cast<int>(pos_.y - height_ / 2.0f), width_, height_};
    SDL_RenderCopy(game_.getRenderer(), texture_, nullptr, &rect);
}

void SunShine::clean()
{
    Object::clean();
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
    }
    if (clicked_sound_)
    {
        Mix_FreeChunk(clicked_sound_);
    }
}

void SunShine::clicked(SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if (glm::distance(glm::vec2(event.button.x, event.button.y), pos_) < width_ / 2.0f && !clicked_)
        {
            clicked_ = true;
            setSpeed(8 * speed_);
            setDest(glm::vec2((TOP_BAR_START_X + TOP_BAR_CARD_START_X) / 2.0f, TOP_BAR_CARD_HEIGHT / 2.0f));
            // 播放音效
            Mix_PlayChannel(-1, clicked_sound_, 0);
        }
    }
}

void initSunshineFilePath()
{
    for (int i = 0; i < 29; i++)
    {
        sunshine_file_path.push_back("res/sunshine/" + std::to_string(i + 1) + ".png");
    }
}
