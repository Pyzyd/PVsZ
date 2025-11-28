#ifndef __GAME_H__
#define __GAME_H__

#include <SDL.h>
#include <glm/glm.hpp>
#include <string>

#define WINDOW_START_X 100
#define WINDOW_START_Y 0

class Scene; // 前向声明

class Game
{
    glm::vec2 screen_size_ = glm::vec2(0.0f);

    Scene* current_scene_ = nullptr; // 当前场景

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;

    bool is_running_ = false;

    Uint64 FPS_ = 60;         // 游戏帧率
    Uint64 frame_delay_ = 0;  // 帧延迟
    float dt_ = 0.0f;         // 帧间隔
public:
    void run();

    void init(std::string title, int width, int height);
    void handleEvents();
    void update(float dt);
    void render();
    void clean();

    static Game& getInstance(){
        static Game instance;
        return instance;
    }

    // getters and setters
    glm::vec2 getScreenSize() const { return screen_size_; } // 获取屏幕大小
    SDL_Renderer* getRenderer() const { return renderer_; } // 获取渲染器

    // 工具函数
    void renderBackground(SDL_Texture* texture);

private:
    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};


#endif // __GAME_H__
