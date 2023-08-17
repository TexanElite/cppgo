#include <iostream>

#include "core/board.hpp"
#include "core/color.hpp"
#include "core/engine.hpp"
#include "core/exception.hpp"
#include "core/game.hpp"
#include "core/piece.hpp"
#include "core/position.hpp"

using namespace cppgo::core;

int main() {
    Game game(BoardSize::NINE);
    Engine engine;
    while (true) {
        std::cout << game.print_to_string() << std::endl;
        std::cout << "Your turn: ";
        std::string move;
        std::cin >> move;
        Position pos{move[0] - 97U, move[1] - 49U};
        // std::cout << pos.x << " " << pos.y << std::endl;
        game.make_move(Move{pos});
        game.make_move(engine.get_best_move(game));
    }
    return 0;
}