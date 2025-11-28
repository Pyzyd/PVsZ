#ifndef __GAME_H__
#define __GAME_H__

#include <SDL.h>
#include <string>

class Game
{
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;

    bool is_running_ = false;
    float dt_ = 0.0f;
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

    ~Game() = default;

private:
    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};


#endif // __GAME_H__
