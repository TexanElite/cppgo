#include "game.hpp"

#include <gtest/gtest.h>

using namespace cppgo::core;

TEST(GameTest, InitializeGame) {
    Game game(BoardSize::NINETEEN);
    EXPECT_EQ(game.turn(), Color::BLACK);
}

TEST(GameTest, MakeMove) {
    Game game(BoardSize::NINETEEN);
    game.makeMove({1, 1});
    EXPECT_EQ(game.turn(), Color::WHITE);
    EXPECT_EQ((game.board()[{1, 1}]), Color::BLACK);
    EXPECT_THROW(game.makeMove({1, 1}), IllegalMoveException);
    game.unmakeMove();
    EXPECT_EQ((game.board()[{1, 1}]), Color::NONE);
}