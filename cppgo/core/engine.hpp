#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>

#include "exception.hpp"
#include "game.hpp"
#include "position.hpp"

namespace cppgo::core {

class SearchTreeNode {
   public:
    SearchTreeNode(GameState game_state, std::shared_ptr<SearchTreeNode> parent)
        : _game_state{game_state}, _wins{0}, _visits{0}, _parent{parent} {}

    constexpr GameState &game_state() { return _game_state; }

    constexpr int &wins() { return _wins; }
    constexpr int &visits() { return _visits; }

    inline std::unordered_map<Move, std::shared_ptr<SearchTreeNode>>
        &children() {
        return _children;
    }
    inline int num_children() const { return _children.size(); }

    inline std::shared_ptr<SearchTreeNode> parent() const { return _parent; }

   private:
    GameState _game_state;
    int _wins;
    int _visits;
    std::unordered_map<Move, std::shared_ptr<SearchTreeNode>> _children;
    std::shared_ptr<SearchTreeNode> _parent;
};

class Engine {
   public:
    Engine() : _gen{_rd()} {};
    Move get_best_move(Game &game) {
        using namespace std::chrono_literals;
        std::shared_ptr<SearchTreeNode> root_node =
            std::make_shared<SearchTreeNode>(game.game_state(), nullptr);
        std::chrono::system_clock::time_point start =
            std::chrono::system_clock::now();
        int count = 0;
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now() - start) < 10000ms) {
            std::shared_ptr<SearchTreeNode> selected_node = select(root_node);
            std::shared_ptr<SearchTreeNode> expanded_node =
                expand(selected_node);
            simulate(expanded_node);
            backpropagate(expanded_node);
            count++;
        }
        std::cout << count << std::endl;
        double best_score = -1;
        Move best_move{Position{-1U, -1U}};

        for (auto &[move, node] : root_node->children()) {
            double score = static_cast<double>(node->wins()) / node->visits();
            std::cout << node->wins() << " / " << node->visits() << " -> " << score << std::endl;
            if (score > best_score) {
                best_score = score;
                best_move = move;
            }
        }
        return best_move;
    }

   private:
    std::random_device _rd;
    std::mt19937 _gen;

    std::shared_ptr<SearchTreeNode> select(
        std::shared_ptr<SearchTreeNode> node) {
        std::vector<Move> legal_moves = node->game_state().get_legal_moves();
        if (node->num_children() == 0 ||
            node->num_children() != legal_moves.size()) {
            return node;
        }
        std::uniform_int_distribution<> distribution(0, legal_moves.size() - 1);
        return select(node->children()[legal_moves[distribution(_gen)]]);
    }

    std::shared_ptr<SearchTreeNode> expand(
        std::shared_ptr<SearchTreeNode> leaf) {
        std::vector<Move> legal_moves = leaf->game_state().get_legal_moves();
        std::vector<Move> remaining_moves;
        for (Move &move : legal_moves) {
            if (leaf->children().find(move) == leaf->children().end()) {
                remaining_moves.push_back(move);
            }
        }
        if (remaining_moves.size() == 0) {
            // TODO
        } else {
            std::uniform_int_distribution<> distribution(
                0, remaining_moves.size() - 1);
            Move &chosen_move = remaining_moves[distribution(_gen)];
            std::shared_ptr<SearchTreeNode> new_node =
                std::make_shared<SearchTreeNode>(
                    leaf->game_state().make_move(chosen_move), leaf);
            leaf->children()[chosen_move] = new_node;
            return new_node;
        }
    }

    void simulate(std::shared_ptr<SearchTreeNode> node) {
        GameState cur = node->game_state();
        std::vector<Move> legal_moves;
        while (!(legal_moves = cur.get_legal_moves()).empty()) {
            std::uniform_int_distribution<> distribution(
                0, legal_moves.size() - 1);
            cur = cur.make_move(legal_moves[distribution(_gen)]);
        }
        int black = 0;
        int white = 0;
        for (unsigned int x = 0; x < cur.board.size(); ++x) {
            for (unsigned int y = 0; y < cur.board.size(); ++y) {
                if (cur.board[{x, y}] == Color::BLACK) {
                    ++black;
                } else if (cur.board[{x, y}] == Color::WHITE) {
                    ++white;
                }
            }
        }
        if (node->game_state().turn == Color::BLACK) {
            node->wins() += (black > white) ? 1 : 0;
        } else {
            node->wins() += (white > black) ? 1 : 0;
        }
        ++node->visits();
        if (node->visits() > 1) {
            throw IllegalColorException();
        }
    }

    void backpropagate(std::shared_ptr<SearchTreeNode> node) {
        std::shared_ptr<SearchTreeNode> ancestor = node->parent();
        bool reverse = true;
        while (ancestor != nullptr) {
            ancestor->wins() += reverse ? node->visits() - node->wins() : node->wins();
            ancestor->visits() += node->visits();
            ancestor = ancestor->parent();
            reverse ^= true;
        }
    }
};
}  // namespace cppgo::core

#endif