#ifndef COLOR_HPP
#define COLOR_HPP

namespace cppgo::core {
enum Color {
    NONE,
    BLACK,
    WHITE,
};

constexpr Color inverse_color(Color color) {
    return Color(WHITE - color + BLACK);
}
}

#endif