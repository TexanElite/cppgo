#ifndef GAME_HPP
#define GAME_HPP

#include <deque>
#include <sstream>

#include "board.hpp"
#include "color.hpp"
#include "exception.hpp"
#include "move.hpp"
#include "piece.hpp"

namespace cppgo::core {

struct GameState {
    Position ko{-1U, -1U};
    Color turn{Color::NONE};
    Board board;
    Board prev_board;

    GameState make_move(const Move &move) {
        if (board[move.position] != Color::NONE) {
            throw IllegalMoveException();  // Occupied
        }
        GameState new_game_state{move.position, inverse_color(turn), board,
                                 board};
        Board &new_board = new_game_state.board;
        new_board[move.position] = turn;
        for (Position &adj_position : move.position.get_adj_positions()) {
            if (new_board.is_position_in_board(adj_position) &&
                new_board[adj_position] == inverse_color(turn) &&
                new_board.is_group_surrounded(adj_position)) {
                new_board.remove_group(adj_position);
            }
        }
        if (new_board.is_group_surrounded(move.position)) {
            throw IllegalMoveException();  // Suicide
        }
        return new_game_state;
    }

    bool is_move_legal(const Move &move) {
        try {
            make_move(move);
            return true;
        } catch (IllegalMoveException e) {
            return false;
        }
    }

    std::vector<Move> get_pseudolegal_moves() {
        std::vector<Move> moves;
        for (unsigned int x = 0; x < board.size(); x++) {
            for (unsigned int y = 0; y < board.size(); y++) {
                const Position cur_position = {x, y};
                if (board[cur_position] == Color::NONE) {
                    moves.push_back(Move{cur_position});
                }
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_moves() {
        std::vector<Move> pseudolegal_moves = get_pseudolegal_moves();
        std::vector<Move> legal_moves;
        for (Move &move : pseudolegal_moves) {
            if (is_move_legal(move)) {
                legal_moves.push_back(move);
            }
        }
        return legal_moves;
    }
};

class Game {
   public:
    Game() = default;
    Game(const BoardSize board_size)
        : _game_states({GameState{Position{-1U, -1U}, Color::BLACK,
                                  Board(board_size), Board(board_size)}}) {}

    inline GameState &game_state() { return _game_states.back(); }

    inline Color turn() { return game_state().turn; }

    inline void make_move(const Move &move) {
        GameState &&new_game_state = game_state().make_move(move);
        _game_states.push_back(new_game_state);
    }

    inline void unmake_move() { _game_states.pop_back(); }

    inline bool is_move_legal(const Move &move) {
        return game_state().is_move_legal(move);
    }

    inline std::vector<Move> get_pseudolegal_moves() {
        return game_state().get_pseudolegal_moves();
    }

    inline std::vector<Move> get_legal_moves() {
        return game_state().get_legal_moves();
    }

    std::string print_to_string() {
        Board &cur_board = game_state().board;
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
    std::deque<GameState> _game_states;
};
}  // namespace cppgo::core

#endif