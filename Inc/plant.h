#ifndef __PLANT_H
#define __PLANT_H

#include "object.h"
#include "actor.h"
#include <map>
#include <string>
#include <vector>

enum class PlantType
{
    PEA,
    SUNFLOWER,
    COUNT,
    NONE = -1,
};

void intiPlantFilePath();

extern std::map<PlantType, std::string> card_file_path;
extern std::map<PlantType, std::vector<std::string>> plant_file_path;

class Plant : public Actor
{
    PlantType p_type_;
    int frame_count_ = 0;
    int frame_index_ = 0;
    int fps_ = 15;
    float frame_timer_ = 0.0f;
    float attack_timer_ = 0.0f;
    float attack_interval_ = 2.0f;
    bool is_attacking_ = false;
    int hurt_damage_ = 0;
    float hurt_timer_ = 0.0f;
    float hurt_interval_ = 0.5f;
    bool is_hurt_ = false;
    bool is_dead_ = false;
public:
    static Plant *addPlantChild(Object *parent, PlantType type, glm::vec2 pos);

    virtual void init() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;

    void takeDamage(int damage) override;
    void die(float dt) override;
    void hurt(float dt);

    void setPlantHealth(int health);

    // getters and setters
    bool isAttacking() const { return is_attacking_; }
    void setIsAttacking(bool is_attacking) { is_attacking_ = is_attacking; }
    PlantType getPlantType() const { return p_type_; }
    SDL_Texture *getPlantTexture() const { return texture_; }
    int getFrameCount() const { return frame_count_; }
    int getFrameIndex() const { return frame_index_; }

    void setPlantType(PlantType type) { p_type_ = type; }
    void setFrameCount(int count) { frame_count_ = count; }
    void setFrameIndex(int index) { frame_index_ = index; }

    void attack();
};

class Card : public Object
{
    glm::vec2 pos_ = glm::vec2(0, 0);
    PlantType p_type_ = PlantType::NONE;
    int bar_index_ = -1;
    

public:
    static Card *addCardChild(Object *parent, PlantType type, glm::vec2 pos);

    virtual void init() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;
    // getters and setters
    PlantType getPlantType() const { return p_type_; }
    glm::vec2 getPos() const { return pos_; }
    int getBarIndex() const { return bar_index_; }
    void setPlantType(PlantType type) { p_type_ = type; }
    void setPos(glm::vec2 pos) { pos_ = pos; }
    void setBarIndex(int index) { bar_index_ = index; }
};

#endif /* __PLANT_H */
