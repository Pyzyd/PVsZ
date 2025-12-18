#include "plant.h"
#include "scene_main.h"
#include "bullet.h"
#include <SDL_image.h>

std::map<PlantType, std::string> card_file_path;
std::map<PlantType, std::vector<std::string>> plant_file_path;

Plant *Plant::addPlantChild(Object *parent, PlantType type, glm::vec2 pos)
{
    Plant *child = new Plant();
    child->setPlantType(type);
    child->init();
    child->setPos(pos);
    if (parent){
        child->setParent(parent);
        parent->addChild(child);
    }
    return child;
}

void Plant::init()
{
    Actor::init();
    o_type_ = ObjectType::PLANT;
    setPlantHealth(100);
    if (this->p_type_ == PlantType::COUNT || this->p_type_ == PlantType::NONE){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid plant type");
        return;
    }
    texture_ = IMG_LoadTexture(game_.getRenderer() , plant_file_path[this->p_type_][frame_index_].c_str());
    if (texture_ == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
        return;
    }
    frame_count_ = static_cast<int>(plant_file_path[this->p_type_].size());
    SDL_QueryTexture(texture_, nullptr, nullptr, &width_, &height_);
}

void Plant::update(float dt)
{
    Actor::update(dt);
    frame_timer_ += dt;
    attack_timer_ += dt;
    if (frame_timer_ >= 1.0f / fps_)
    {
        frame_index_++;
        if (frame_index_ >= frame_count_)
        {
            frame_index_ = 0;
        }
        frame_timer_ = 0.0f;
        texture_ = IMG_LoadTexture(game_.getRenderer() , plant_file_path[this->p_type_][frame_index_].c_str());
    }
    if (is_hurt_){
        hurt(dt);
    }
    if (is_dead_){
        die(dt);
    }
    if (is_attacking_){
        if (attack_timer_ >= attack_interval_){
            attack();
            attack_timer_ = 0.0f;
        }
    }
}

void Plant::render()
{
    SDL_FRect dst = {this->pos_.x - width_ / 2.0, this->pos_.y - height_ / 2.0, static_cast<float>(width_), static_cast<float>(height_)};
    SDL_RenderCopyF(game_.getRenderer(), texture_, nullptr, &dst);
    Actor::render();
}

void Plant::clean()
{
    Actor::clean();
    if (texture_ != nullptr){
        SDL_DestroyTexture(texture_);
    }
}

void Plant::takeDamage(int damage)
{
    hurt_damage_ = damage;
    if (health_ > 0){
        is_hurt_ = true;
    }else{
        is_dead_ = true;
        is_hurt_ = false;
    }
}

void Plant::die(float dt)
{
    setActive(false);
    setAlive(false);
    setNeedRemove(true);
}

void Plant::hurt(float dt)
{
    hurt_timer_ += dt;
    if (hurt_timer_ > hurt_interval_){
        health_ -= hurt_damage_;
        hurt_timer_ = 0.0f;
    }
}

void Plant::setPlantHealth(int health)
{
    switch(p_type_)
    {
    case PlantType::PEA:
        health_ = 100;
        break;
    case PlantType::SUNFLOWER:
        health_ = 125;
        break;
    case PlantType::COUNT:
    case PlantType::NONE:
    default:
        break;
    }
}

void Plant::attack()
{
    switch (p_type_)
    {
    case PlantType::PEA:
        Bullet::addBulletChild(this->parent_, PlantType::PEA, glm::vec2(pos_.x + width_ / 2.0, pos_.y - height_ / 4.0));
        break;
    
    case PlantType::SUNFLOWER:
    case PlantType::COUNT:
    case PlantType::NONE:
    default:
        break;
    }
}

void intiPlantFilePath()
{
    std::string path;
    for (int i = 0; i < static_cast<int>(PlantType::COUNT); ++i){
        card_file_path[static_cast<PlantType>(i)] = "res/Cards/card_" + std::to_string(i+1) + ".png";
        for (int j = 0; j < 20; ++j){
            path = "res/zhiwu/" + std::to_string(i) + "/" + std::to_string(j+1) + ".png";
            if (fileExists(path)){
                plant_file_path[static_cast<PlantType>(i)].push_back(path);
            }
            else{
                break;
            }
        }
    }
}

Card *Card::addCardChild(Object* parent, PlantType type, glm::vec2 pos)
{
    Card *child = new Card();
    child->setPlantType(type);
    child->setPos(pos);
    child->init();
    if (parent){
        child->setParent(parent);
        parent->addChild(child);
    }
    return child;
}

void Card::init()
{
    Object::init();
    o_type_ = ObjectType::CARD;
    if (this->p_type_ == PlantType::COUNT || this->p_type_ == PlantType::NONE){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid plant type");
        return;
    }
    texture_ = IMG_LoadTexture(game_.getRenderer() , card_file_path[this->p_type_].c_str());
    if (texture_ == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
        return;
    }
    bar_index_ = static_cast<int>((pos_.x - TOP_BAR_CARD_START_X) / TOP_BAR_CARD_INTERVAL);
    SDL_QueryTexture(texture_, nullptr, nullptr, &width_, &height_);
}

void Card::update(float dt)
{
    Object::update(dt);
}

void Card::render()
{
    Object::render();
    SDL_FRect dst = {this->pos_.x, this->pos_.y, static_cast<float>(width_), static_cast<float>(height_)};
    SDL_RenderCopyF(game_.getRenderer(), texture_, nullptr, &dst);
}

void Card::clean()
{
    Object::clean();
    if (texture_ != nullptr){
        SDL_DestroyTexture(texture_);
    }
}
