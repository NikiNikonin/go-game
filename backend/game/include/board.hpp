#pragma once
#include "./enums.hpp"
#include "./stone_group.hpp"
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

class Board {
  private:
    std::vector<std::vector<State>> _board;
    std::vector<std::vector<State>> _lastBoard;
    std::unordered_set<StoneGroup> _stone_groups;
    PlayerColor _curr_player;
    int _size;
    bool _doubleSkip = false;

    bool isKoViolation(std::vector<std::vector<State>>&);
    bool endGame();

  public:
    Board(int);
    ~Board();

    bool makeMove(std::string);
    void huy();
    const std::vector<std::vector<std::string>> getBoard() const;
};