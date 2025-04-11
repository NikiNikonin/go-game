#pragma once
#include "./enums.hpp"
#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

class Board {
  private:
    std::vector<std::vector<PointColor>> _board;
    std::string _result;
    int _size;

    void endGame();

  public:
    PlayerColor _curr_player;
    Board(int);
    ~Board();

    void makeMove(std::string);
    std::vector<std::string> getValidMoves(PlayerColor);
    std::vector<std::string> getValidMoves();
    bool isMoveValid(std::string, PlayerColor);
    bool isMoveValid(std::string);
    int evaluateBoard(PlayerColor);
    int alphaBeta(int, int, int, bool, PlayerColor);
    std::string getBestMove(int, PlayerColor);
    std::string getBestMove(int);
    std::string getRandomMove(PlayerColor);
    std::string getRandomMove();

    std::string result() const;
    const std::vector<std::vector<std::string>> getPointBoard() const;
    void operator=(const Board& other);
};