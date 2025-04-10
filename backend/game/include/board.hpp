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
#include <stack>

class Board {
  private:
    std::vector<std::vector<PointColor>> _board_lines;
    std::vector<std::vector<std::vector<AreaColor>>> _board_area;
    PlayerColor _curr_player;
    std::string _result;
    int _size;

    void endGame();
    std::map<char, int> countTerritory();

  public:
    Board(int);
    ~Board();

    bool makeMove(std::string);
    std::string result() const;
    const std::vector<std::vector<std::string>> getPointBoard() const;
};