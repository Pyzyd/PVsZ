#include "plant.h"
#include <SDL_image.h>

std::map<PLANT_TYPE, std::string> card_file_path;
std::map<PLANT_TYPE, std::vector<std::string>> plant_file_path;


void Plant::init()
{
    Object::init();
    if (this->type_ == PLANT_TYPE::COUNT || this->type_ == PLANT_TYPE::NONE){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid plant type");
        return;
    }
}

void Plant::update(float dt)
{
    Object::update(dt);
}

void Plant::render()
{
    Object::render();
}

void initFilePath()
{
    for (int i = 0; i < static_cast<int>(PLANT_TYPE::COUNT); ++i){
        card_file_path[static_cast<PLANT_TYPE>(i)] = "res/Cards/card_" + std::to_string(i+1) + ".png";
    }
    // for (int i = 0; i < static_cast<int>(PLANT_TYPE::COUNT); ++i){
        
    // }
}

Card *Card::addCardChild(Object* parent, PLANT_TYPE type, glm::vec2 pos)
{
    Card *child = new Card();
    child->setType(type);
    child->setPos(pos);
    child->init();
    parent->addChild(child);
    return child;
}

void Card::init()
{
    Object::init();
    if (this->type_ == PLANT_TYPE::COUNT || this->type_ == PLANT_TYPE::NONE){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid plant type");
        return;
    }
    card_texture_ = IMG_LoadTexture(game_.getRenderer() , card_file_path[this->type_].c_str());
    if (card_texture_ == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", SDL_GetError());
        return;
    }
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
