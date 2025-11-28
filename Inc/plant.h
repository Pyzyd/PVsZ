#ifndef __PLANT_H
#define __PLANT_H

#include "object.h"
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

void initFilePath();

extern std::map<PlantType, std::string> card_file_path;
extern std::map<PlantType, std::vector<std::string>> plant_file_path;

class Plant : public Object
{
    PlantType p_type_;
    SDL_Texture *plant_texture_ = nullptr;
    glm::vec2 pos_ = glm::vec2(0, 0);
    Object *parent_ = nullptr;
    int frame_count_ = 0;
    int frame_index_ = 0;

public:
    static Plant *addPlantChild(Object *parent, PlantType type, glm::vec2 pos);

    virtual void init() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;
    // getters and setters
    PlantType getPlantType() const { return p_type_; }
    glm::vec2 getPos() const { return pos_; }
    Object *getParent() const { return parent_; }
    SDL_Texture *getPlantTexture() const { return plant_texture_; }
    int getFrameCount() const { return frame_count_; }
    int getFrameIndex() const { return frame_index_; }

    void setPlantType(PlantType type) { p_type_ = type; }
    void setPos(glm::vec2 pos)
    {
        int w, h;
        SDL_QueryTexture(plant_texture_, nullptr, nullptr, &w, &h);
        pos_ = pos - glm::vec2(w / 2, h / 2);
    }
    void setParent(Object *parent) { parent_ = parent; }
    void setFrameCount(int count) { frame_count_ = count; }
    void setFrameIndex(int index) { frame_index_ = index; }
};

class Card : public Object
{
    Object *parent_ = nullptr;
    glm::vec2 pos_ = glm::vec2(0, 0);
    PlantType p_type_ = PlantType::NONE;
    SDL_Texture *card_texture_ = nullptr;
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
