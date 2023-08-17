#include "core/game.hpp"

#include <gtest/gtest.h>

using namespace cppgo::core;

TEST(GameTest, InitializeGame) {
    Game game(BoardSize::NINETEEN);
    EXPECT_EQ(game.turn(), Color::BLACK);
}

TEST(GameTest, MakeMove) {
    Game game(BoardSize::NINETEEN);
    game.make_move({1, 1});
    EXPECT_EQ(game.turn(), Color::WHITE);
    EXPECT_EQ((game.game_state().board[{1, 1}]), Color::BLACK);
    EXPECT_THROW(game.make_move({1, 1}), IllegalMoveException);
    game.unmake_move();
    EXPECT_EQ((game.game_state().board[{1, 1}]), Color::NONE);
}