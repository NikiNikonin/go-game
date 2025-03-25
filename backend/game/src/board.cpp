#include "../include/board.hpp"

Board::Board(int s) : _stone_groups({}), _curr_player(PlayerColor::Black) {
    int sizes[3] = {9, 13, 19};
    _size = sizes[s];
    _board = std::vector<std::vector<State>>(_size, std::vector<State>(_size, State::Neutral));
    std::cout << "nu pizdec\n";
}

Board::~Board() {
    std::cout << "nu ahuet'\n";
}

bool Board::isKoViolation(std::vector<std::vector<State>>& board) {
    return board == _lastBoard;
}

bool Board::makeMove(std::string move) {
    if (move == "skip") {
        _curr_player = (_curr_player == PlayerColor::White ? PlayerColor::Black : PlayerColor::White);
        if (_doubleSkip)
            endGame();
        else
            _doubleSkip = !_doubleSkip;
        return true;
    }

    if (move.size() != 2 && move.size() != 3) return false;

    if (move.front() >= 'J') --move.front();
    int row = move.front() - 'A';
    if (row < 0 || row >= _size) return false;
    int col = std::stoi(move.substr(1)) - 1;
    if (col < 0 || col >= _size) return false;
    std::cout << row << ' ' << col << '\n';

    if (_board[row][col] != State::Neutral) return false;

    bool took_someone;

    // for (int i = 0; i < _size; ++i) {
    //     for (int j = 0; j < _size; ++j) std::cout << char(_board[i][j]) << ' ';
    //     std::cout << '\n';
    // }

    std::vector<std::vector<State>> tmp_board = _board;

    // std::cout << "\n\n";
    // for (int i = 0; i < _size; ++i) {
    //     for (int j = 0; j < _size; ++j) std::cout << char(tmp_board[i][j]) << ' ';
    //     std::cout << '\n';
    // }

    std::unordered_set<StoneGroup> tmp_stone_groups = _stone_groups;
    tmp_board[row][col] = State(_curr_player);
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    std::vector<StoneGroup> new_groups({StoneGroup({row, col}, &_board)}), will_be_dead;
    for (const auto& dir : directions) {
        int x = (row + dir.first < 0 ? 0 : (row + dir.first >= _size ? _size : row + dir.first));
        int y = (col + dir.second < 0 ? 0 : (col + dir.second >= _size ? _size : col + dir.second));
        if (_board[x][y] == State::Neutral) continue;
        if (_board[x][y] == State(_curr_player)) {
            for (const auto& group : tmp_stone_groups)
                if (group.isIn(x, x)) {
                    new_groups.push_back(group);
                    break;
                }
        } else
            for (auto& group : tmp_stone_groups)
                if (group.isIn(x, x)) {
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

    huy();
    // for(auto i : _stone_groups){
    //     for(auto j : i.constStonesRef()){
    //         std::cout << j.first << ' ' << j.second << " | ";
    //     }
    //     std::cout << '\n';
    // }

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

void Board::huy() {
    std::cout << "\n\n";
    for (int i = 0; i < _size; ++i) {
        for (int j = 0; j < _size; ++j) std::cout << char(_board[i][j]) << ' ';
        std::cout << '\n';
    }
}