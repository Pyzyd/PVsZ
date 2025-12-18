#include "zombie.h"
#include "scene_main.h"
#include <SDL_image.h>

std::vector<std::string> zombie_move_files;
std::vector<std::string> zombie_die_files;
std::vector<std::string> zombie_eat_files;

void initZombieFilePath()
{
    for (int i = 0; i < 22; i++)
    {
        zombie_move_files.push_back("res/zm/" + std::to_string(i + 1) + ".png");
        if (i < 21)
        {
            zombie_eat_files.push_back("res/zm_eat/" + std::to_string(i + 1) + ".png");
            if (i < 20){
                zombie_die_files.push_back("res/zm_dead/" + std::to_string(i + 1) + ".png");
            }
        }
    }
}

Zombie *Zombie::addZombieChild(Object *parent, glm::vec2 pos, glm::ivec2 coor, float speed)
{
    Zombie *zombie = new Zombie();
    zombie->setPos(pos);
    zombie->setCoor(coor);
    zombie->setSpeed(speed);
    zombie->init();
    if (parent)
    {
        zombie->setParent(parent);
        parent->addChild(zombie);
    }
    return zombie;
}

void Zombie::init()
{
    Actor::init();
    o_type_ = ObjectType::ZOMBIE;
    health_ = 100;
    texture_ = IMG_LoadTexture(game_.getRenderer(), zombie_move_files[frame_index_].c_str());
    if (!texture_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
    }
    SDL_QueryTexture(texture_, nullptr, nullptr, &width_, &height_);
    frame_count_ = static_cast<int>(zombie_move_files.size());
}

void Zombie::handleEvents(SDL_Event &event)
{
    Actor::handleEvents(event);
}

void Zombie::update(float dt)
{
    Actor::update(dt);
    pos_.x = pos_.x - speed_ * dt;
    coor_ = SceneMain::posToMapCoor(pos_);
    if (pos_.x < -width_)
    {
        parent_->setActive(false);
        return;
    }
    if (alive_){
        if (is_eating_){
            eat(dt);
        }else{
            move(dt);
        }
    }else{
        die(dt);
    }
}

void Zombie::render()
{
    Actor::render();
    SDL_Rect rect = {static_cast<int>(pos_.x - width_* 3.0f / 4.0f), static_cast<int>(pos_.y - height_ / 2.0f), width_, height_};
    SDL_RenderCopy(game_.getRenderer(), texture_, nullptr, &rect);
}

void Zombie::clean()
{
    Actor::clean();
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void Zombie::move(float dt)
{
    frame_timer_ += dt;
    if (frame_timer_ >= frame_delay_)
    {
        frame_timer_ = 0;
        frame_index_++;
        if (frame_index_ >= frame_count_)
        {
            frame_index_ = 0;
        }
        texture_ = IMG_LoadTexture(game_.getRenderer(), zombie_move_files[frame_index_].c_str());
        if (!texture_)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
        }
    }
}

void Zombie::eat(float dt)
{
    frame_timer_ += dt;
    if (frame_timer_ >= frame_delay_)
    {
        frame_timer_ = 0;
        frame_index_++;
        if (frame_index_ >= frame_count_)
        {
            frame_index_ = 0;
        }
        texture_ = IMG_LoadTexture(game_.getRenderer(), zombie_eat_files[frame_index_].c_str());
        if (!texture_)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
        }
    }
}

void Zombie::takeDamage(int damage)
{
    health_ -= damage;
    if (alive_ && health_ <= 0)
    {
        setAlive(false);
        speed_ = 0;
        frame_timer_ = 0;
        frame_index_ = 0;
        frame_count_ = static_cast<int>(zombie_die_files.size());
    }
}

void Zombie::die(float dt)
{
    frame_timer_ += dt;
    if (frame_timer_ >= frame_delay_)
    {
        frame_timer_ = 0;
        frame_index_++;
        if (frame_index_ >= frame_count_)
        {
            setActive(false);
            setNeedRemove(true);
            return;
        }
        texture_ = IMG_LoadTexture(game_.getRenderer(), zombie_die_files[frame_index_].c_str());
        if (!texture_)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
        }
    }
}

void Zombie::hurt()
{
}
