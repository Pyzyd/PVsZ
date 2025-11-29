#include "plant.h"
#include "scene_main.h"

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
    Object::init();
    o_type_ = ObjectType::PLANT;
    if (this->p_type_ == PlantType::COUNT || this->p_type_ == PlantType::NONE){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid plant type");
        return;
    }
    plant_texture_ = IMG_LoadTexture(game_.getRenderer() , plant_file_path[this->p_type_][frame_index_].c_str());
    if (plant_texture_ == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
        return;
    }
    frame_count_ = static_cast<int>(plant_file_path[this->p_type_].size());
    
}

void Plant::update(float dt)
{
    Object::update(dt);
    frame_timer_ += dt;
    if (frame_timer_ >= 1.0f / fps_)
    {
        frame_index_++;
        if (frame_index_ >= frame_count_)
        {
            frame_index_ = 0;
        }
        frame_timer_ = 0.0f;
        plant_texture_ = IMG_LoadTexture(game_.getRenderer() , plant_file_path[this->p_type_][frame_index_].c_str());
    }
}

void Plant::render()
{
    Object::render();
    SDL_FRect dst = {this->pos_.x, this->pos_.y, 0, 0};
    int w, h;
    SDL_QueryTexture(plant_texture_, nullptr, nullptr, &w, &h);
    dst.w = w;
    dst.h = h;
    SDL_RenderCopyF(game_.getRenderer(), plant_texture_, nullptr, &dst);
}

void Plant::clean()
{
    Object::clean();
    if (plant_texture_ != nullptr){
        SDL_DestroyTexture(plant_texture_);
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
    card_texture_ = IMG_LoadTexture(game_.getRenderer() , card_file_path[this->p_type_].c_str());
    if (card_texture_ == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
        return;
    }
    bar_index_ = static_cast<int>((pos_.x - TOP_BAR_CARD_START_X) / TOP_BAR_CARD_INTERVAL);
}

void Card::update(float dt)
{
    Object::update(dt);
}

void Card::render()
{
    Object::render();
    SDL_FRect dst = {this->pos_.x, this->pos_.y, 0, 0};
    int w, h;
    SDL_QueryTexture(card_texture_, nullptr, nullptr, &w, &h);
    dst.w = w;
    dst.h = h;
    SDL_RenderCopyF(game_.getRenderer(), card_texture_, nullptr, &dst);
}

void Card::clean()
{
    Object::clean();
    if (card_texture_ != nullptr){
        SDL_DestroyTexture(card_texture_);
    }
}
