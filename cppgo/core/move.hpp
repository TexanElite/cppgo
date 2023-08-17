#ifndef MOVE_HPP
#define MOVE_HPP

#include "position.hpp"

namespace cppgo::core {

struct Move {
    Position position;
    constexpr bool operator==(const Move& other) const {
        return position == other.position;
    }
};

};  // namespace cppgo::core

namespace std {
template <>
struct hash<cppgo::core::Move> {
    size_t operator()(cppgo::core::Move const& move) const noexcept {
        return hash<cppgo::core::Position>{}(move.position);
    }
};
}  // namespace std

#endif