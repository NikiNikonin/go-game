#pragma once
#include "./board.hpp"
#include "./enums.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Player {
  protected:
    PlayerColor color;
    std::shared_ptr<Board> board;

  public:
    Player(char, std::shared_ptr<Board> b);
    virtual ~Player() = default;
    int score = 0;
};
