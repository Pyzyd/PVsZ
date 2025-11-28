#include "game.h"

int main(int argc, char *argv[]) {
    auto& game = Game::getInstance();
    game.init("P_VS_Z", 900, 600);
    game.run();
    game.clean();
    return 0;
}