#ifndef BOARD_HPP
#define BOARD_HPP

#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "color.hpp"
#include "position.hpp"

namespace cppgo::core {
enum BoardSize : unsigned int { NINE = 9, THIRTEEN = 13, NINETEEN = 19 };

struct Board {
   public:
    Board() : Board(BoardSize::NINETEEN) {}
    Board(const BoardSize board_size)
        : _board_size{board_size}, _piece_board{board_size * board_size} {}
    Board(const Board &board)
        : _board_size{board._board_size}, _piece_board{board._piece_board} {}

    constexpr BoardSize size() const { return _board_size; }

    constexpr Color &operator[](const Position &&position) {
        return _piece_board[position_to_board_index(position)];
    }

    constexpr Color &operator[](const Position &position) {
        return _piece_board[position_to_board_index(position)];
    }

    constexpr friend bool operator==(const Board &lhs, const Board &rhs) {
        return lhs._board_size == rhs._board_size && lhs._piece_board == rhs._piece_board;
    }

    constexpr bool is_position_in_board(const Position &position) const {
        return position.x < _board_size && position.y < _board_size;
    }

    std::vector<Position> get_group(const Position &position) {
        std::unordered_set<Position> visited;
        std::vector<Position> group;
        Color color = _piece_board[position_to_board_index(position)];
        if (color == Color::NONE) {
            return group;
        }
        get_group_helper(position, color, visited, group);
        return group;
    }

    int get_group_liberties(const Position &position) {
        std::unordered_set<Position> liberties;
        for (Position &stone : get_group(position)) {
            for (Position &adj_position : stone.get_adj_positions()) {
                if (is_position_in_board(adj_position) &&
                    _piece_board[position_to_board_index(adj_position)] ==
                        Color::NONE) {
                    liberties.insert(adj_position);
                }
            }
        }
        return liberties.size();
    }

    bool is_group_surrounded(const Position &position) {
        return get_group_liberties(position) == 0;
    }

    void remove_group(const Position &position) {
        for (Position &stone : get_group(position)) {
            _piece_board[position_to_board_index(stone)] = Color::NONE;
        }
    }

   private:
    BoardSize _board_size;
    std::vector<Color> _piece_board;

    constexpr size_t position_to_board_index(const Position &position) const {
        return position.x + position.y * _board_size;
    }

    void get_group_helper(const Position position, const Color color,
                          std::unordered_set<Position> &visited,
                          std::vector<Position> &group) {
        auto [_, inserted] = visited.insert(position);
        if (inserted) {
            if (_piece_board[position_to_board_index(position)] == color) {
                group.push_back(position);
                for (Position &adj_position : position.get_adj_positions()) {
                    if (is_position_in_board(adj_position)) {
                        get_group_helper(adj_position, color, visited, group);
                    }
                }
            }
        }
    }
};
}  // namespace cppgo::core

#endif