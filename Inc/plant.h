#ifndef __PLANT_H
#define __PLANT_H

#include "object.h"
#include <map>
#include <string>
#include <vector>

enum class PLANT_TYPE
{
    PEA,
    SUNFLOWER,
    COUNT,
    NONE = -1,
};

void initFilePath();

extern std::map<PLANT_TYPE, std::string> card_file_path;
extern std::map<PLANT_TYPE, std::vector<std::string>> plant_file_path;

class Plant : public Object
{
    PLANT_TYPE type_;

public:
    virtual void init() override;
    virtual void update(float dt) override;
    virtual void render() override;
    // getters and setters
    PLANT_TYPE getType() const { return type_; }
    void setType(PLANT_TYPE type) { type_ = type; }
};

class Card : public Object
{
    Object *parent_ = nullptr;
    glm::vec2 pos_ = glm::vec2(0, 0);
    PLANT_TYPE type_ = PLANT_TYPE::NONE;
    SDL_Texture *card_texture_ = nullptr;

public:
    static Card *addCardChild(Object *parent, PLANT_TYPE type, glm::vec2 pos);

    virtual void init() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;
    // getters and setters
    PLANT_TYPE getType() const { return type_; }
    glm::vec2 getPos() const { return pos_; }
    void setType(PLANT_TYPE type) { type_ = type; }
    void setPos(glm::vec2 pos) { pos_ = pos; }
};

#endif /* __PLANT_H */
