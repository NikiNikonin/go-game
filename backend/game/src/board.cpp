#include "../include/board.hpp"

Board::Board(int s) : _stone_groups({}), _curr_player(PlayerColor::Black) {
    int sizes[3] = {9, 13, 19};
    _size = sizes[s];
    _board = std::vector<std::vector<State>>(_size, std::vector<State>(_size, State::Neutral));
}

Board::~Board() {
    std::cout << "pizda\n";
}

bool Board::isKoViolation(std::vector<std::vector<State>>& board) {
    return board == _lastBoard;
}

bool Board::makeMove(std::string move) {
    std::cout << move << '\n';
    if (move == "skip") {
        _curr_player = (_curr_player == PlayerColor::White ? PlayerColor::Black : PlayerColor::White);
        if (_doubleSkip)
            endGame();
        else
            _doubleSkip = !_doubleSkip;
        return true;
    }

    if (move.size() != 2 && move.size() != 3) return false;

    int row = move.front() - 'a';
    if (row < 0 || row >= _size) return false;
    int col = std::stoi(move.substr(1));
    if (col < 0 || col >= _size) return false;

    if (_board[row][col] != State::Neutral) return false;

    // BEFORE HERE ALL OK

    bool took_someone;
    std::vector<std::vector<State>> tmp_board = _board;
    std::unordered_set<StoneGroup> tmp_stone_groups = _stone_groups;
    tmp_board[row][col] = State(_curr_player);
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    std::vector<StoneGroup> new_groups({StoneGroup({row, col}, &_board)}), will_be_dead;
    for (const auto& dir : directions) {
        if (_board[row + dir.first][col + dir.second] == State::Neutral) continue;
        if (_board[row + dir.first][col + dir.second] == State(_curr_player)) {
            for (const auto& group : tmp_stone_groups)
                if (group.isIn(row + dir.first, col + dir.second)) {
                    new_groups.push_back(group);
                    break;
                }
        } else
            for (auto& group : tmp_stone_groups)
                if (group.isIn(row + dir.first, col + dir.second)) {
                    if (group.liberty() == 1)
                        will_be_dead.push_back(group);
                    else
                        group.decrLiverty1();
                    break;
                }
    }
    took_someone = !will_be_dead.empty();
    for (auto& dead : will_be_dead) tmp_stone_groups.erase(dead);
    StoneGroup new_stone_group(State(_curr_player), &_board);
    for (auto& group : new_groups) {
        tmp_stone_groups.erase(group);
        new_stone_group.addGroup(std::move(group));
    }
    if (new_stone_group.liberty() == 0 && !took_someone) return false;
    tmp_stone_groups.insert(new_stone_group);
    will_be_dead.clear();
    for (auto& group : tmp_stone_groups) {
        group.updateLiberty();
        if (group.liberty() == 0) will_be_dead.push_back(group);
    }
    for (auto& dead : will_be_dead) tmp_stone_groups.erase(dead);

    if (isKoViolation(tmp_board)) return false;
    _stone_groups = std::move(tmp_stone_groups);
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

bool Board::endGame() {
    return true;
}