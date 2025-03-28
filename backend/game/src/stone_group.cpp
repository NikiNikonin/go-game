#include "../include/stone_group.hpp"

StoneGroup::StoneGroup(State c, std::vector<std::vector<State>>* b) : _stones({}), _liberty(0), _board(b), _color(c) {
}

StoneGroup::StoneGroup(std::pair<int, int> p, std::vector<std::vector<State>>* b) : _stones({p}), _board(b), _color((*_board)[p.first][p.second]) {
    updateLiberty();
}
StoneGroup::~StoneGroup() = default;

int StoneGroup::liberty() const {
    return _liberty;
}

int& StoneGroup::libertyRef() {
    return _liberty;
}

const std::set<std::pair<int, int>>& StoneGroup::constStonesRef() const {
    return _stones;
}

State StoneGroup::color() const {
    return _color;
}

void StoneGroup::clearStones() {
    for (auto i : _stones) (*_board)[i.first][i.second] = State::Neutral;
}

void StoneGroup::setBoardPtr(std::vector<std::vector<State>>* new_board) const {
    _board = new_board;
}

void StoneGroup::updateLiberty() const {
    std::set<std::pair<int, int>> visited;
    std::pair<int, int> start = *_stones.begin();
    int liberties = 0;

    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::function<void(std::pair<int, int>)> explore = [&](std::pair<int, int> pos) {
        auto [r, c] = pos;
        if (r < 0 || r >= _board->size() || c < 0 || c >= _board->size()) return;

        if (visited.count({r, c}) != 0) return;
        visited.insert({r, c});

        if ((*_board)[r][c] == State::Neutral)
            ++liberties;
        else if ((*_board)[r][c] == _color)
            for (const auto& dir : directions) explore({r + dir.first, c + dir.second});
    };

    explore(start);
    _liberty = liberties;
}

void StoneGroup::addStone(std::pair<int, int> s) {
    if (_stones.find(s) != _stones.end()) return;
    _stones.insert(s);
    updateLiberty();
}

void StoneGroup::addGroup(StoneGroup& other) {
    _stones.merge(other._stones);
    updateLiberty();
}

bool StoneGroup::isIn(int row, int col) const {
    return _stones.find({row, col}) != _stones.end();
}

bool StoneGroup::operator==(const StoneGroup& other) const {
    return _stones == other._stones && _liberty == other._liberty && _color == other._color;
}

void StoneGroup::operator=(StoneGroup& other) {
    _stones = other._stones;
    _liberty = other._liberty;
    _color = other._color;
}

void StoneGroup::decrLiverty1() const {
    --_liberty;
}