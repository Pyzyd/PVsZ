#include "game.h"

int main(int argc, char *argv[]) {
    auto& game = Game::getInstance();
    game.init("My SDL Game", 800, 600);
    game.run();
    game.clean();
    return 0;
}