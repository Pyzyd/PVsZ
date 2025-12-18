#include "bullet.h"
#include "zombie.h"
#include <SDL_image.h>

std::map<PlantType, std::string> bullet_file_path;
std::map<PlantType, std::string> bullet_explosion_file;

Bullet* Bullet::addBulletChild(Object* parent, PlantType planttype, glm::vec2 pos,glm::vec2 dir, float speed)
{
    Bullet* bullet = new Bullet();
    bullet->setPlantType(planttype);
    bullet->setPos(pos);
    bullet->setDir(dir);
    bullet->setSpeed(speed);
    bullet->init();
    if (parent != nullptr){
        bullet->setParent(parent);
        parent->addChild(bullet);
    }
    return bullet;
}

void Bullet::init()
{
    Object::init();
    is_explosion_ = false;
    o_type_ = ObjectType::PLANT_BULLET;
    switch (planttype_)
    {
    case PlantType::PEA:
        texture_ = IMG_LoadTexture(game_.getRenderer(), bullet_file_path[planttype_].c_str());
        break;
    case PlantType::SUNFLOWER:
    case PlantType::NONE:
    case PlantType::COUNT:
    default:
        break;
    }
    if (texture_){
        SDL_QueryTexture(texture_, nullptr, nullptr, &width_, &height_);
    }
}

void Bullet::update(float dt)
{
    Object::update(dt);
    pos_ += dir_ * speed_ * dt;
    if (texture_ && is_explosion_){
        speed_ = 0;
        texture_ = IMG_LoadTexture(game_.getRenderer(), bullet_explosion_file[planttype_].c_str());
        explosion_timer_ += dt;
    }
    if (explosion_timer_ > explosion_duration_ && (is_explosion_ || pos_.x > game_.getScreenSize().x)){
        setNeedRemove(true);
    }
}

void Bullet::render()
{
    Object::render();
    if (texture_){
        SDL_FRect dst_rect = {pos_.x - width_ / 2.0, pos_.y - height_ / 2.0, static_cast<float>(width_), static_cast<float>(height_)};
        SDL_RenderCopyF(game_.getRenderer(), texture_, nullptr, &dst_rect);
    }
}

void Bullet::clean()
{
    Object::clean();
    if (texture_ != nullptr)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void Bullet::causeHarm(Zombie *zombie)
{
    if (zombie->getActive()){
        zombie->takeDamage(damage_);
    }
}

void initBulletFilePath()
{
    bullet_file_path[PlantType::PEA] = "res/bullets/PeaNormal/PeaNormal_0.png";
    bullet_explosion_file[PlantType::PEA] = "res/bullets/PeaNormalExplode/PeaNormalExplode_0.png";
}
