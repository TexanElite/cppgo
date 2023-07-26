#include "board.hpp"

#include <gtest/gtest.h>

#include <algorithm>

using namespace cppgo::core;

TEST(BoardTest, InitializeBoard) {
    Board board(BoardSize::NINETEEN);
    EXPECT_EQ(board.size(), BoardSize::NINETEEN);
    for (unsigned int x = 0; x < BoardSize::NINETEEN; x++) {
        for (unsigned int y = 0; y < BoardSize::NINETEEN; y++) {
            Color &color = board[{x, y}];
            EXPECT_EQ(color, Color::NONE);
        }
    }
}

TEST(BoardTest, GetGroup) {
    Board board(BoardSize::NINETEEN);
    std::array<Position, 5> black_positions = {Position{0, 0}, Position{1, 0},
                                               Position{1, 1}, Position{1, 2},
                                               Position{2, 1}};
    std::array<Position, 5> white_positions = {Position{0, 1}, Position{0, 2},
                                               Position{0, 3}, Position{1, 3},
                                               Position{2, 3}};
    for (Position &position : black_positions) {
        board[position] = Color::BLACK;
    }
    for (Position &position : white_positions) {
        board[position] = Color::WHITE;
    }

    std::vector<Position> group = board.get_group(black_positions[0]);
    EXPECT_EQ(group.size(), 5);
    for (Position &position : black_positions) {
        EXPECT_NE(std::find(group.begin(), group.end(), position), group.end());
    }
    EXPECT_EQ(board.get_group_liberties(black_positions[0]), 3);

    group = board.get_group(white_positions[0]);
    EXPECT_EQ(group.size(), 5);
    for (Position &position : white_positions) {
        EXPECT_NE(std::find(group.begin(), group.end(), position), group.end());
    }
    EXPECT_EQ(board.get_group_liberties(white_positions[0]), 5);

    board.remove_group(black_positions[0]);
    for (Position &position : black_positions) {
        EXPECT_EQ(board[position], Color::NONE);
    }
}