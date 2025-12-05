#ifndef __Bullet_H
#define __Bullet_H

#include "object.h"
#include "plant.h"
#include <map>
#include <string>

extern std::map<PlantType, std::string> bullet_file_path;
extern std::map<PlantType, std::string> bullet_explosion_file;

void initBulletFilePath();


class Zombie;
class Bullet : public Object
{
    PlantType planttype_ = PlantType::NONE;
    int damage_ = 10;
    glm::vec2 pos_ = glm::vec2(0, 0);
    glm::vec2 dir_ = glm::vec2(0, 0);
    float speed_ = 0;
    bool is_explosion_ = false;
    float explosion_timer_ = 0.0f;
    float explosion_duration_ = 0.15f;
public:

    static Bullet* addBulletChild(Object* parent, PlantType planttype, glm::vec2 pos, glm::vec2 dir = glm::vec2(1, 0), float speed = 240.0f);

    virtual void init() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    // getter and setter
    PlantType getPlantType() { return planttype_; }
    int getDamage() { return damage_; }
    glm::vec2 getPos() { return pos_; }
    glm::vec2 getDir() { return dir_; }
    float getSpeed() { return speed_; }
    bool isExplosion() { return is_explosion_; }
    void setExplosion(bool is_explosion) { is_explosion_ = is_explosion; }
    void setPos(glm::vec2 pos) { pos_ = pos; }
    void setDir(glm::vec2 dir) { dir_ = dir; }
    void setSpeed(float speed) { speed_ = speed; }
    void setDamage(int damage) { damage_ = damage; }
    void setPlantType(PlantType planttype) { planttype_ = planttype; }

    void causeHarm(Zombie* zombie);
};


#endif /* __Bullet_H */
