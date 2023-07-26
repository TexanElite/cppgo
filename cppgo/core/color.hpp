#ifndef COLOR_HPP
#define COLOR_HPP

#include <assert.h>

namespace cppgo::core {
enum Color {
    NONE,
    BLACK,
    WHITE,
};

constexpr Color inverse_color(Color color) {
    if (color == NONE) {
        throw IllegalColorException();
    }

    return Color(WHITE - color + BLACK);
}
}

#endif