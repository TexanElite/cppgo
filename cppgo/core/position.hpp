#ifndef POSITION_HPP
#define POSITION_HPP

#include <boost/functional/hash.hpp>
#include <limits>
#include <string>

namespace cppgo::core {
struct Position {
    unsigned int x;
    unsigned int y;

    constexpr std::string to_sgf() {
        return std::string(1, static_cast<char>(x + 'a')) +
               std::string(1, static_cast<char>(y + 'a'));
    }

    constexpr bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    constexpr Position north() const { return Position{x, y - 1}; }

    constexpr Position south() const { return Position{x, y + 1}; }

    constexpr Position east() const { return Position{x + 1, y}; }

    constexpr Position west() const { return Position{x - 1, y}; }

    constexpr std::vector<Position> get_adj_positions() const {
        return std::vector({north(), south(), east(), west()});
    }
};
}  // namespace cppgo::core

namespace std {
template <>
struct hash<cppgo::core::Position> {
    size_t operator()(
        cppgo::core::Position const& position) const noexcept {
        size_t seed = 0;
        boost::hash_combine(seed, position.x);
        boost::hash_combine(seed, position.y);
        return seed;
    }
};
}  // namespace std

#endif