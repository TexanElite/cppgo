#ifndef PIECE_HPP
#define PIECE_HPP

#include "color.hpp"

namespace cppgo::core {
struct Piece {
    Color color;
    Position position;
};
}  // namespace cppgo::core

#endif