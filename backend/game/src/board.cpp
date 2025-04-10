#include "../include/board.hpp"

Board::Board(int s) : _curr_player(PlayerColor::Black), _result("none") {
    int sizes[3] = {9, 13, 19};
    _size = sizes[s];
    _board_lines = std::vector<std::vector<PointColor>>(_size, std::vector<PointColor>(_size, PointColor::Neutral));
    _board_area = std::vector<std::vector<std::vector<AreaColor>>>(
        _size - 1, std::vector<std::vector<AreaColor>>(_size - 1, std::vector<AreaColor>(4, AreaColor::Neutral)));
}

Board::~Board() = default;

bool Board::makeMove(std::string move) {
    if (_result != "none") return false;

    if (move.size() != 2 && move.size() != 3) return false;

    if (move.front() >= 'J') --move.front();
    int row = move.front() - 'A';
    if (row < 0 || row >= _size) return false;
    int col = std::stoi(move.substr(1)) - 1;
    if (col < 0 || col >= _size) return false;

    if (_board_lines[row][col] != PointColor::Neutral) return false;

    std::vector<std::pair<int, int>> directions = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

    _board_lines[row][col] = PointColor(_curr_player);
    std::vector<std::vector<bool>> visited(_size, std::vector<bool>(_size, false));

    std::stack<std::pair<int, int>> s;
    s.push({row, col});
    visited[row][col] = true;
    while (!s.empty()) {
        auto v = s.top();
        s.pop();
        for (auto dir : directions) {
            int x = row + dir.first, y = col + dir.second;
            if (x < 0 || x >= _size || y < 0 || y >= _size || visited[x][y] || _board_lines[x][y] != PointColor(_curr_player)) continue;
            s.push({x, y});
        }
    }

    return true;
}

const std::vector<std::vector<std::string>> Board::getPointBoard() const {
    std::vector<std::vector<std::string>> string_board(_size, std::vector<std::string>(_size));
    for (int i = 0; i < _size; ++i)
        for (int j = 0; j < _size; ++j)
            string_board[i][j] =
                (_board_lines[i][j] == PointColor::White ? "White" : (_board_lines[i][j] == PointColor::Black ? "Black" : "Neutral"));

    return string_board;
}

std::string Board::result() const {
    return _result;
}

std::map<char, int> Board::countTerritory() {
    std::map<char, int> territory = {{'w', 0}, {'b', 0}};

    for (const auto& i : _board_area)
        for (const auto& j : i)
            for (const auto& k : j)
                if (k == AreaColor::White || k == AreaColor::Black) ++territory[char(k)];

    return territory;
}

void Board::endGame() {
}