#include "scene_main.h"
#include "plant.h"
#include "sunshine.h"
#include "zombie.h"
#include "bullet.h"
#include "scene_start.h"
#include "asset_store.h"
#include <SDL_image.h>

void SceneMain::init()
{
    Scene::init();
    // background_ = IMG_LoadTexture(game_.getRenderer(), "res/bg.jpg");
    background_ = game_.getAssetStore()->getImage("res/bg.jpg");
    if (!background_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    // top_bar_ = IMG_LoadTexture(game_.getRenderer(), "res/bar5.png");
    top_bar_ = game_.getAssetStore()->getImage("res/bar5.png");
    if (!top_bar_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
    }
    intiPlantFilePath();    // 初始化文件路径
    initSunshineFilePath(); // 初始化阳光文件路径
    initZombieFilePath();   // 初始化僵尸文件路径
    initBulletFilePath();   // 初始化子弹文件路径
    card_num_ = (TOP_BAR_CARD_NUM > static_cast<int>(PlantType::COUNT) ? static_cast<int>(PlantType::COUNT) : TOP_BAR_CARD_NUM);
    for (int i = 0; i < card_num_; ++i)
    {
        Card::addCardChild(shared_from_this(), static_cast<PlantType>(i), glm::vec2(TOP_BAR_CARD_START_X + i * TOP_BAR_CARD_INTERVAL, TOP_BAR_CARD_START_Y));
    }
    for (auto &row : plant_map_)
    {
        row.fill(false);
    }
    font_ = TTF_OpenFont("C:/Windows/Fonts/seguibl.ttf", 20);
    sunshine_collector_pos_ = glm::vec2((TOP_BAR_START_X + TOP_BAR_CARD_START_X) / 2.0f, TOP_BAR_CARD_HEIGHT / 2.0f);
    for (int i = 0; i < PLANT_MAP_GRID_ROWS; ++i)
    {
        has_zombie_.push_back(false);
    }
}

void SceneMain::handleEvents(SDL_Event &event)
{
    Scene::handleEvents(event);
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            auto scene = std::make_shared<SceneStart>();
            game_.changeScene(scene);
        }
    }
    userClickedCard(event);
}

void SceneMain::update(float dt)
{
    Scene::update(dt);
    createRandomSunShine(dt);
    createZombie(dt);
    updatePlant();
    updateZombie();

    if (!is_active_)
    {
        game_.changeScene(std::make_shared<SceneStart>());
    }
}

void SceneMain::render()
{
    game_.renderBackground(background_, BG_MAIN_START_X, BG_START_Y);
    renderTopBar();
    renderSunShineNum();
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
        clicked_card_plant_ = nullptr;
    }
    if (font_)
    {
        TTF_CloseFont(font_);
        font_ = nullptr;
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

void SceneMain::renderSunShineNum()
{
    auto text = std::to_string(total_sunshine_num_ * SunShine::value);
    SDL_Color color = {0, 0, 0, 255}; // 设置文本颜色为黑色
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game_.getRenderer(), surface);
    SDL_Rect rect = {SUNSHINE_NUM_TEXT_CENTER_X - surface->w / 2, SUNSHINE_NUM_TEXT_CENTER_Y - surface->h / 2, surface->w, surface->h};
    SDL_RenderCopy(game_.getRenderer(), texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void SceneMain::updatePlant()
{
    plantAttackZombie();
    for (auto &child : children_)
    {
        if (child->getObjectType() == ObjectType::PLANT_BULLET)
        {
            
            auto bullet = std::dynamic_pointer_cast<Bullet>(child);
            if (bullet != nullptr)
            {
                ZombieTakeDamage(bullet);
            }
        }
    }
}

void SceneMain::updateZombie()
{
    ZombieEatPlant();
    int count = 0;
    for (auto &child : children_)
    {
        if (child->getObjectType() == ObjectType::ZOMBIE)
        {
            auto zombie = std::dynamic_pointer_cast<Zombie>(child);
            if (zombie != nullptr)
            {
                if (zombie->getCoor().x >= 0 && zombie->getCoor().x <= PLANT_MAP_WIDTH &&
                    zombie->getCoor().y >= 0 && zombie->getCoor().y <= PLANT_MAP_HEIGHT)
                {
                    count++;
                }
            }
        }
    }
    zombie_num_ = count;
}

void SceneMain::userClickedCard(SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        // 检查鼠标点击位置是否在卡牌区域内
        if (event.button.x > TOP_BAR_CARD_START_X && event.button.x < TOP_BAR_CARD_START_X + TOP_BAR_CARD_INTERVAL * card_num_ &&
            event.button.y > TOP_BAR_CARD_START_Y && event.button.y < TOP_BAR_CARD_START_Y + TOP_BAR_CARD_HEIGHT)
        {
            int index = (event.button.x - TOP_BAR_CARD_START_X) / TOP_BAR_CARD_INTERVAL; // 计算点击的卡牌索引
            // SDL_Log("%d", index);  // 调试输出卡牌索引
            card_clicked_ = true;       // 设置卡牌被点击标志为true
            setClickedCardPlant(index); // 设置被点击的卡牌对应的植物
        }
    }
    // 处理鼠标移动事件，且卡牌已被点击
    else if (event.type == SDL_MOUSEMOTION && card_clicked_ == true)
    {
        if (clicked_card_plant_ != nullptr) // 如果存在被点击的植物
        {
            clicked_card_plant_->setPos(game_.getMousePos());
        }
    }
    // 处理鼠标左键释放事件，且卡牌已被点击
    else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && card_clicked_ == true)
    {
        card_clicked_ = false; // 重置卡牌点击标志
        if (!clicked_card_plant_)
        { // 如果没有被点击的植物，直接返回
            return;
        }
        // 检查鼠标位置是否在植物种植区域内
        int x = game_.getMousePos().x;
        int y = game_.getMousePos().y;
        if (x > PLANT_MAP_START_X && x < PLANT_MAP_START_X + PLANT_MAP_WIDTH &&
            y > PLANT_MAP_START_Y && y < PLANT_MAP_START_Y + PLANT_MAP_HEIGHT)
        {
            int row = (y - PLANT_MAP_START_Y) / PLANT_MAP_GRID_H; // 计算种植的行索引
            int col = (x - PLANT_MAP_START_X) / PLANT_MAP_GRID_W; // 计算种植的列索引
            if (plant_map_[row][col] == false)
            {                                // 如果该位置没有植物
                plant_map_[row][col] = true; // 标记该位置已被占用
                glm::vec2 pos;
                pos.x = PLANT_MAP_START_X + col * PLANT_MAP_GRID_W + PLANT_MAP_GRID_W / 2;
                pos.y = PLANT_MAP_START_Y + PLANT_MAP_GRID_H / 8.0 + row * PLANT_MAP_GRID_H + PLANT_MAP_GRID_H / 3;
                clicked_card_plant_->setPos(pos);
                clicked_card_plant_->setCoor(glm::ivec2(col, row));
                clicked_card_plant_ = nullptr; // 清空被点击的植物指针
            }
            else
            {                                             // 如果该位置已有植物
                clicked_card_plant_->setNeedRemove(true); // 设置植物需要移除
            }
        }
        else // 如果鼠标不在种植区域内
        {
            if (clicked_card_plant_ != nullptr) // 如果存在被点击的植物
            {
                clicked_card_plant_->setNeedRemove(true); // 设置植物需要移除
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
                auto card = std::dynamic_pointer_cast<Card>(child);
                if (card->getBarIndex() == index)
                {
                    clicked_card_plant_ = Plant::addPlantChild(shared_from_this(), card->getPlantType(), game_.getMousePos());
                    break;
                }
            }
        }
    }
}

void SceneMain::createRandomSunShine(float dt)
{
    if (sunshine_num_ < RANDOM_SUNSHINE_NUMS)
    {
        sunshine_timer_ += dt;
        if (sunshine_timer_ >= sunshine_interval_)
        {
            glm::vec2 pos;
            pos.x = game_.getRandomFloat(PLANT_MAP_START_X, PLANT_MAP_START_X + PLANT_MAP_WIDTH);
            pos.y = game_.getRandomFloat(-TOP_BAR_CARD_HEIGHT, TOP_BAR_START_Y);
            SunShine::addSunshineChild(shared_from_this(), pos, glm::vec2(pos.x, game_.getRandomFloat(PLANT_MAP_START_Y, PLANT_MAP_START_Y + PLANT_MAP_HEIGHT)));
            sunshine_num_++;
            sunshine_timer_ = 0;
        }
    }
}

void SceneMain::countTotalSunShine()
{
    // for (auto &child : children_)
    // {
    //     if (child->getObjectType() == ObjectType::SUNSHINE)
    //     {
    //         auto sunshine = std::dynamic_pointer_cast<std::shared_ptr<SunShine> >(child);
    //         if (sunshine->isClicked() && !sunshine->isCollected()){
    //             sunshine->setClicked(false);
    //             total_sunshine_num_++;
    //             sunshine_num_--;
    //         }
    //     }
    // }
    total_sunshine_num_++;
    sunshine_num_--;
    if (sunshine_num_ < 0)
    {
        sunshine_num_ = 0;
    }
}

void SceneMain::createZombie(float dt)
{
    zombie_timer_ += dt;
    if (zombie_timer_ > zombie_interval_ && zombie_num_ < ZOMBIE_NUM_MAX)
    {
        zombie_timer_ = 0;
        glm::vec2 pos;
        pos.x = game_.getRandomFloat(PLANT_MAP_START_X + PLANT_MAP_WIDTH, PLANT_MAP_START_X + PLANT_MAP_WIDTH + PLANT_MAP_GRID_W);
        pos.y = PLANT_MAP_START_Y + PLANT_MAP_GRID_H / 4.0 + game_.getRandomInt(0, PLANT_MAP_GRID_ROWS - 1) * PLANT_MAP_GRID_H;
        Zombie::addZombieChild(shared_from_this(), pos, posToMapCoor(pos));
        zombie_interval_ = game_.getRandomFloat(3.0, 8.0);
    }
}

void SceneMain::plantAttackZombie()
{
    // ZombieEmerge();
    // for (int i = 0; i < PLANT_MAP_GRID_ROWS; i++)
    // {
    //     if (has_zombie_[i])
    //     {
    //         for (int j = 0; j < PLANT_MAP_GRID_COLS; j++)
    //         {
    //             if (plant_map_[i][j])
    //             {
    //                 auto plant = getPlantFromMapCoor(glm::ivec2(j, i));
    //                 if (plant != nullptr)
    //                 {
    //                     plant->setIsAttacking(true);
    //                 }
    //             }
    //         }
    //     }
    // }
    for (int i = 0; i < PLANT_MAP_GRID_ROWS; i++)
    {
        for (int j = 0; j < PLANT_MAP_GRID_COLS; j++)
        {
            if (plant_map_[i][j])
            {
                auto plant = getPlantFromMapCoor(glm::ivec2(j, i));
                bool is_attacking = false;
                for (auto &child : children_)
                {
                    if (child->getObjectType() == ObjectType::ZOMBIE)
                    {
                        auto zombie = std::dynamic_pointer_cast<Zombie>(child);
                        if (zombie->getCoor().x >= j && zombie->getCoor().y == i)
                        {
                            is_attacking = true;
                            break;
                        }
                    }
                }
                plant->setIsAttacking(is_attacking);
            }
        }
    }
}

// void SceneMain::ZombieEmerge()
// {
//     for (int i = 0; i < PLANT_MAP_GRID_ROWS; i++)
//     {
//         for (int j = 0; j < PLANT_MAP_GRID_COLS; j++)
//         {
//             auto zombie = getZombieFromMapCoor(glm::ivec2(j, i));
//             if (zombie != nullptr)
//             {
//                 has_zombie_[i] = true;
//                 break;
//             }
//             has_zombie_[i] = false;
//         }
//     }
// }

void SceneMain::ZombieTakeDamage(std::shared_ptr<Bullet> bullet)
{
    for (auto &child : children_)
    {
        if (child->getObjectType() == ObjectType::ZOMBIE)
        {
            auto zombie = std::dynamic_pointer_cast<Zombie>(child);
            if (zombie->isAlive() && !(bullet->isExplosion()) && glm::length(zombie->getPos() - bullet->getPos()) < zombie->getWidth() * 0.4f)
            {
                zombie->takeDamage(bullet->getDamage());
                bullet->setExplosion(true);
            }
        }
    }
}

void SceneMain::ZombieEatPlant()
{
    for (auto &child : children_)
    {
        if (child->getObjectType() == ObjectType::ZOMBIE)
        {
            auto zombie = std::dynamic_pointer_cast<Zombie>(child);
            if (zombie->isAlive())
            {
                auto plant = getPlantFromMapCoor(zombie->getCoor());
                if (plant != nullptr)
                {
                    if (plant->isAlive())
                    {
                        plant->takeDamage(zombie->getDamage());
                        if (!zombie->getIsEating())
                        {
                            zombie->setIsEating(true);
                        }
                    }
                    else
                    {
                        zombie->setIsEating(false);
                        plant_map_[plant->getCoor().y][plant->getCoor().x] = false;
                    }
                }
            }
        }
    }
}

std::shared_ptr<Plant> SceneMain::getPlantFromMapCoor(glm::ivec2 map_coor)
{
    for (auto &child : children_)
    {
        if (child->getObjectType() == ObjectType::PLANT)
        {
            auto plant = std::dynamic_pointer_cast<Plant>(child);
            if (plant->getCoor() == map_coor)
            {
                return plant;
            }
        }
    }
    return nullptr;
}

std::shared_ptr<Zombie> SceneMain::getZombieFromMapCoor(glm::ivec2 map_coor)
{
    for (auto &child : children_)
    {
        if (child->getObjectType() == ObjectType::ZOMBIE)
        {
            auto zombie = std::dynamic_pointer_cast<Zombie>(child);
            if (zombie->getCoor() == map_coor)
            {
                return zombie;
            }
        }
    }
    return nullptr;
}

glm::ivec2 SceneMain::posToMapCoor(glm::vec2 pos)
{
    glm::ivec2 coor;
    coor.x = (pos.x - PLANT_MAP_START_X) / PLANT_MAP_GRID_W;
    coor.y = (pos.y - PLANT_MAP_START_Y) / PLANT_MAP_GRID_H;
    if (coor.x < 0 || coor.x >= PLANT_MAP_GRID_COLS || coor.y < 0 || coor.y >= PLANT_MAP_GRID_ROWS)
    {
        return glm::ivec2(-1, -1);
    }
    return coor;
}
