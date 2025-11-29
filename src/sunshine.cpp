#include "sunshine.h"
#include <SDL_image.h>

std::vector<std::string> sunshine_file_path;

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
}

void SunShine::update(float dt)
{
    Object::update(dt);
    pos_.y = pos_.y + speed_ * dt;
    if (pos_.y >= dest_.y)
    {
        speed_ = 0;
    }
    frame_time_ += dt;
    if (frame_time_ >= 1.0 / fps_)
    {
        frame_time_ = 0.0;
        frame_index_++;
        if (frame_index_ >= sunshine_file_path.size())
        {
            frame_index_ = 0;
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
    SDL_Rect rect = {static_cast<int>(pos_.x), static_cast<int>(pos_.y), 0, 0};
    SDL_QueryTexture(texture_, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(game_.getRenderer(), texture_, nullptr, &rect);
}

void SunShine::clean()
{
    Object::clean();
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
    }
}

void initSunshineFilePath()
{
    for (int i = 0; i < 29; i++)
    {
        sunshine_file_path.push_back("res/sunshine/" + std::to_string(i + 1) + ".png");
    }
}
