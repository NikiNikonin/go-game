#pragma once
#include "./enums.hpp"
#include "./stone_group.hpp"
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

class Board {
  private:
    std::vector<std::vector<State>> _board;
    std::vector<std::vector<State>> _lastBoard;
    std::unordered_set<StoneGroup> _stone_groups;
    PlayerColor _curr_player;
    int _size, _b_captured, _w_captured;
    std::string _result;
    bool _double_skip = false;

    bool isKoViolation(std::vector<std::vector<State>>&);
    void endGame();
    public:
    std::map<char, int> countTerritory();

  public:
    Board(int);
    ~Board();

    bool makeMove(std::string);
    std::string result() const;
    const std::vector<std::vector<std::string>> getBoard() const;
};