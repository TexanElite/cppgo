#ifndef COLOR_HPP
#define COLOR_HPP

#include <assert.h>

#include "exception.hpp"

namespace cppgo::core {
enum Color {
    NONE,
    BLACK,
    WHITE,
};

constexpr Color inverse_color(Color color) {
    if (color == Color::NONE) {
        throw IllegalColorException();
    }
    return Color(WHITE - color + BLACK);
}
}  // namespace cppgo::core

#endif