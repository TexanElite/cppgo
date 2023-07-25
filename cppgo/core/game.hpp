#ifndef GAME_HPP
#define GAME_HPP

#include <deque>
#include <sstream>

#include "board.hpp"
#include "color.hpp"
#include "exception.hpp"
#include "piece.hpp"

namespace cppgo::core {

struct GameState {
    Color turn;
    GameState() : turn{Color::BLACK} {};
    GameState(Color color) : turn{color} {};
};

class Game {
   public:
    Game() = default;
    Game(const BoardSize board_size) : _board_queue({Board(board_size)}), _game_state_queue{GameState()} {}
    void makeMove(const Position &position) {
        Board &cur_board = _board_queue.front();
        GameState &curGameState = _game_state_queue.front();
        if (cur_board[position] != Color::NONE) {
            throw IllegalMoveException();
        }
        Board new_board(cur_board);
        GameState new_game_state(inverse_color(curGameState.turn));
        new_board[position] = curGameState.turn;
        if (new_board.is_group_surrounded(position)) {
            new_board[position] = Color::NONE;
            throw IllegalMoveException();
        }
        for (Position &adj_position : position.get_adj_positions()) {
            if (new_board.is_position_in_board(adj_position) &&
                new_board.is_group_surrounded(adj_position)) {
                new_board.remove_group(adj_position);
            }
        }
        _board_queue.emplace_back(new_board);
        _game_state_queue.emplace_back(new_game_state);
    }

    void unmakeMove() {
        
    }

    std::string print_to_string() {
        Board &cur_board = _board_queue.front();
        std::ostringstream output;
        output << "  ";
        for (unsigned int y = 0; y < cur_board.size(); y++) {
            output << static_cast<char>(y + 'a');
        }
        output << std::endl;
        for (unsigned int x = 0; x < cur_board.size(); x++) {
            output << (x + 1) << " ";
            for (unsigned int y = 0; y < cur_board.size(); y++) {
                switch (cur_board[{y, x}]) {
                    case Color::NONE: {
                        output << "+";
                        break;
                    }
                    case Color::BLACK: {
                        output << "B";
                        break;
                    }
                    case Color::WHITE: {
                        output << "W";
                        break;
                    }
                }
            }
            output << std::endl;
        }
        return output.str();
    }

   private:
    std::deque<Board> _board_queue;
    std::deque<GameState> _game_state_queue;
};
}  // namespace cppgo::core

#endif