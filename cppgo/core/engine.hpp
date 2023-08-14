#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "exception.hpp"
#include "game.hpp"
#include "position.hpp"

namespace cppgo::core {
class Engine {
   public:
    Engine() = default;
    Position getBestMove(Game &game) {
        Board &board = game.board();
        // TODO: Make an actual evaluation
        for (unsigned int x = 0; x < board.size(); x++) {
            for (unsigned int y = 0; y < board.size(); y++) {
                try {
                    game.makeMove({x, y});
                    game.unmakeMove();
                    return {x, y};
                } catch (IllegalMoveException e) {
                    // Ignore move
                }
            }
        }
    }

   private:
};
}  // namespace cppgo::core

#endif