#include "../include/player.hpp"

Player::Player(char c, std::shared_ptr<Board> b) : board(std::move(b)) {
    if (c == 'w' || c == 'W')
        color = PlayerColor::White;
    else if (c == 'b' || c == 'B')
        color = PlayerColor::Black;
    else
        throw std::invalid_argument("Invalid color: \'" + std::string{c} + "\'. must be \'w\', \'W\', \'b\' or \'B\'.");
}