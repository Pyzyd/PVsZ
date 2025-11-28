#include "scene_main.h"
#include "plant.h"

#include <SDL_image.h>

void SceneMain::init()
{
    Scene::init();
    background_ = IMG_LoadTexture(game_.getRenderer(), "res/bg.jpg");
    if (!background_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    top_bar_ = IMG_LoadTexture(game_.getRenderer(), "res/bar5.png");
    if (!top_bar_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    initFilePath(); // 初始化文件路径
    card_num_ = (TOP_BAR_CARD_NUM > static_cast<int>(PlantType::COUNT) ? static_cast<int>(PlantType::COUNT) : TOP_BAR_CARD_NUM);
    for (int i = 0; i < card_num_; ++i)
    {
        Card::addCardChild(this, static_cast<PlantType>(i), glm::vec2(TOP_BAR_CARD_START_X + i * TOP_BAR_CARD_INTERVAL, TOP_BAR_CARD_START_Y));
    }
    for (auto &row : plant_map_)
    {
        row.fill(false);
    }
}

void SceneMain::handleEvents(SDL_Event &event)
{
    Scene::handleEvents(event);
    userClickedCard(event);
}

void SceneMain::update(float dt)
{
    Scene::update(dt);
}

void SceneMain::render()
{
    game_.renderBackground(background_);
    renderTopBar();
    Scene::render();
}

void SceneMain::clean()
{
    Scene::clean();
    if (!background_)
    {
        SDL_DestroyTexture(background_);
        background_ = nullptr;
    }
    if (!top_bar_)
    {
        SDL_DestroyTexture(top_bar_);
        top_bar_ = nullptr;
    }
    if (clicked_card_plant_ != nullptr)
    {
        clicked_card_plant_->setNeedRemove(true);
    }
}

void SceneMain::renderTopBar()
{
    if (top_bar_)
    {
        SDL_FRect dest = {TOP_BAR_START_X, TOP_BAR_START_Y, 800, 100};
        int w, h;
        SDL_QueryTexture(top_bar_, nullptr, nullptr, &w, &h);
        dest.w = w;
        dest.h = h;
        SDL_RenderCopyF(game_.getRenderer(), top_bar_, nullptr, &dest);
    }
}

void SceneMain::userClickedCard(SDL_Event &event)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    game_.setMousePos(glm::vec2(x, y));
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if (event.button.x > TOP_BAR_CARD_START_X && event.button.x < TOP_BAR_CARD_START_X + TOP_BAR_CARD_INTERVAL * card_num_ &&
            event.button.y > TOP_BAR_CARD_START_Y && event.button.y < TOP_BAR_CARD_START_Y + TOP_BAR_CARD_HEIGHT)
        {
            int index = (event.button.x - TOP_BAR_CARD_START_X) / TOP_BAR_CARD_INTERVAL;
            // SDL_Log("%d", index);
            card_clicked_ = true;
            setClickedCardPlant(index);
        }
    }
    else if (event.type == SDL_MOUSEMOTION && card_clicked_ == true)
    {
        if (clicked_card_plant_ != nullptr)
        {
            clicked_card_plant_->setPos(game_.getMousePos());
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && card_clicked_ == true)
    {
        card_clicked_ = false;
        if (!clicked_card_plant_) {
            return;
        }
        if (x > PLANT_MAP_START_X && x < PLANT_MAP_START_X + PLANT_MAP_WIDTH &&
            y > PLANT_MAP_START_Y && y < PLANT_MAP_START_Y + PLANT_MAP_HEIGHT)
        {
            int row = (y - PLANT_MAP_START_Y) / PLANT_MAP_GRID_H;
            int col = (x - PLANT_MAP_START_X) / PLANT_MAP_GRID_W;
            // SDL_Log("%d %d", row, col);
            if (plant_map_[row][col] == false){
                plant_map_[row][col] = true;
                glm::vec2 pos;
                pos.x = PLANT_MAP_START_X + col * PLANT_MAP_GRID_W + PLANT_MAP_GRID_W / 2;
                pos.y = PLANT_MAP_START_Y + row * PLANT_MAP_GRID_H + PLANT_MAP_GRID_H / 3;
                clicked_card_plant_->setPos(pos);
                clicked_card_plant_ = nullptr;
            }
        }
        else
        {
            if (clicked_card_plant_ != nullptr)
            {
                clicked_card_plant_->setNeedRemove(true);
            }
        }
    }
}

void SceneMain::setClickedCardPlant(int index)
{
    if (index >= 0 && index < card_num_)
    {
        for (auto &child : children_)
        {
            if (child->getObjectType() == ObjectType::CARD)
            {
                auto card = dynamic_cast<Card *>(child);
                if (card->getBarIndex() == index)
                {
                    clicked_card_plant_ = Plant::addPlantChild(this, card->getPlantType(), game_.getMousePos());
                    break;
                }
            }
        }
    }
}
