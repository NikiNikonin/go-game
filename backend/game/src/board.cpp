#include "../include/board.hpp"

Board::Board(int s) : _stone_groups({}), _curr_player(PlayerColor::Black), _b_captured(0), _w_captured(0), _result("none") {
    int sizes[3] = {9, 13, 19};
    _size = sizes[s];
    _board = std::vector<std::vector<State>>(_size, std::vector<State>(_size, State::Neutral));
}

Board::~Board() = default;

bool Board::isKoViolation(std::vector<std::vector<State>>& board) {
    return board == _lastBoard;
}

bool Board::makeMove(std::string move) {
    if (_result != "none") return false;

    if (move == "skip") {
        _curr_player = (_curr_player == PlayerColor::White ? PlayerColor::Black : PlayerColor::White);
        if (_double_skip)
            endGame();
        else
            _double_skip = !_double_skip;
        return true;
    } else
        _double_skip = false;

    if (move.size() != 2 && move.size() != 3) return false;

    if (move.front() >= 'J') --move.front();
    int row = move.front() - 'A';
    if (row < 0 || row >= _size) return false;
    int col = std::stoi(move.substr(1)) - 1;
    if (col < 0 || col >= _size) return false;

    if (_board[row][col] != State::Neutral) return false;

    bool took_someone;
    std::vector<std::vector<State>> tmp_board = _board;
    tmp_board[row][col] = State(_curr_player);
    std::unordered_set<StoneGroup> tmp_stone_groups = _stone_groups;
    for (auto& i : tmp_stone_groups) i.setBoardPtr(&tmp_board);
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    std::vector<StoneGroup> new_groups({StoneGroup({row, col}, &tmp_board)}), will_be_dead;

    for (const auto& dir : directions) {
        int x = row + dir.first;
        int y = col + dir.second;
        if (x < 0 || x >= _size || y < 0 || y >= _size) continue;
        if (_board[x][y] == State::Neutral) continue;

        if (_board[x][y] == State(_curr_player)) {
            for (const auto& group : tmp_stone_groups)
                if (group.isIn(x, y)) {
                    new_groups.push_back(group);
                    break;
                }
        } else
            for (auto& group : tmp_stone_groups)
                if (group.isIn(x, y)) {
                    group.updateLiberty();
                    if (group.liberty() == 0) will_be_dead.push_back(group);
                    break;
                }
    }
    took_someone = !will_be_dead.empty();
    for (auto& dead : will_be_dead) {
        tmp_stone_groups.erase(dead);
        dead.clearStones();
    }
    StoneGroup new_stone_group(State(_curr_player), &tmp_board);
    for (auto& group : new_groups) {
        tmp_stone_groups.erase(group);
        new_stone_group.addGroup(group);
    }
    if (new_stone_group.liberty() == 0 && !took_someone) return false;
    tmp_stone_groups.insert(new_stone_group);
    will_be_dead.clear();
    for (auto& group : tmp_stone_groups) {
        group.updateLiberty();
        if (group.liberty() == 0) will_be_dead.push_back(group);
    }
    for (auto& dead : will_be_dead) {
        dead.clearStones();
        tmp_stone_groups.erase(dead);
    }

    if (isKoViolation(tmp_board)) return false;
    _stone_groups.clear();
    _stone_groups = tmp_stone_groups;
    _lastBoard = std::move(_board);
    _board = std::move(tmp_board);
    _curr_player = (_curr_player == PlayerColor::White ? PlayerColor::Black : PlayerColor::White);

    return true;
}

const std::vector<std::vector<std::string>> Board::getBoard() const {
    std::vector<std::vector<std::string>> string_board(_size, std::vector<std::string>(_size));
    for (int i = 0; i < _size; ++i)
        for (int j = 0; j < _size; ++j)
            string_board[i][j] = (_board[i][j] == State::White ? "White" : (_board[i][j] == State::Black ? "Black" : "Neutral"));

    return string_board;
}

std::string Board::result() const {
    return _result;
}

std::map<char, int> Board::countTerritory() {
    std::map<char, int> territory = {{'w', 0}, {'b', 0}};
    std::vector<std::vector<bool>> visited(_size, std::vector<bool>(_size, false));
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    auto isInside = [&](int x, int y) { return x >= 0 && x < _size && y >= 0 && y < _size; };

    for (int i = 0; i < _size; ++i)
        for (int j = 0; j < _size; ++j) {
            if (_board[i][j] != State::Neutral || visited[i][j]) continue;

            std::set<PlayerColor> surrounding_color;
            std::queue<std::pair<int, int>> q;
            std::vector<std::pair<int, int>> region;
            q.push({i, j});
            visited[i][j] = true;

            while (!q.empty()) {
                auto [x, y] = q.front();
                q.pop();
                region.push_back({x, y});

                for (auto [dx, dy] : directions) {
                    int nx = x + dx, ny = y + dy;
                    if (!isInside(nx, ny)) continue;
                    if (visited[nx][ny]) continue;

                    if (_board[nx][ny] == State::Neutral) {
                        q.push({nx, ny});
                        visited[nx][ny] = true;
                    } else
                        surrounding_color.insert(PlayerColor(_board[nx][ny]));
                }
            }

            if (surrounding_color.size() == 1) territory[char(*surrounding_color.begin())] += region.size();
        }

    return territory;
}

void Board::endGame() {
    auto area = countTerritory();
    int w_points = 13 + (_b_captured + area['w']) * 2;
    int b_points = (_w_captured + area['b']) * 2;
    int delta = abs(w_points - b_points);
    _result = (w_points > b_points ? "White" : "Black");
    _result += " won with a lead of " + std::to_string(delta / 2) + (delta % 2 ? ".5" : "") + " points. (w" + std::to_string(w_points / 2) +
               (w_points % 2 ? ".5" : "") + "; b" + std::to_string(b_points / 2) + (b_points % 2 ? ".5" : "") + ")\n";
}