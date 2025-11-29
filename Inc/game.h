#ifndef __GAME_H__
#define __GAME_H__

#include <SDL.h>
#include <glm/glm.hpp>
#include <string>
#include <random>


class Scene; // 前向声明

class Game
{
    std::random_device rd;
    std::mt19937 gen;

    glm::vec2 screen_size_ = glm::vec2(0.0f);
    glm::vec2 mouse_pos_ = glm::vec2(0.0f);

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
    glm::vec2 getMousePos() const { return mouse_pos_; } // 获取鼠标位置
    void setMousePos(glm::vec2 pos) { mouse_pos_ = pos; } // 设置鼠标位置

    // 工具函数
    void renderBackground(SDL_Texture* texture, int start_x, int start_y);
    void moveMouse();

    void Game::changeScene(Scene *scene);

    float getRandomFloat(float min, float max) {
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
    int getRandomInt(int min, int max) {
        std::uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

private:
    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};


#endif // __GAME_H__
