#include "../include/board.hpp"

Board::Board(int s) : _curr_player(PlayerColor::Black), _result("none") {
    int sizes[3] = {9, 13, 19};
    _size = sizes[s];
    _board = std::vector<std::vector<PointColor>>(_size, std::vector<PointColor>(_size, PointColor::Neutral));

    switch (s) {
    case 0:
        _board[1][1] = PointColor::White;
        _board[2][2] = PointColor::White;
        _board[1][2] = PointColor::Black;
        _board[2][1] = PointColor::Black;

        _board[6][6] = PointColor::Black;
        _board[7][7] = PointColor::Black;
        _board[6][7] = PointColor::White;
        _board[7][6] = PointColor::White;
        break;

    case 1:
        _board[2][2] = PointColor::White;
        _board[3][3] = PointColor::White;
        _board[2][3] = PointColor::Black;
        _board[3][2] = PointColor::Black;

        _board[2][9] = PointColor::White;
        _board[3][10] = PointColor::White;
        _board[2][10] = PointColor::Black;
        _board[3][9] = PointColor::Black;

        _board[9][2] = PointColor::White;
        _board[10][3] = PointColor::White;
        _board[10][2] = PointColor::Black;
        _board[9][3] = PointColor::Black;

        _board[9][9] = PointColor::White;
        _board[10][10] = PointColor::White;
        _board[9][10] = PointColor::Black;
        _board[10][9] = PointColor::Black;

        break;

    case 2:
        _board[0][0] = PointColor::White;
        break;
    }
}

void Board::operator=(const Board& other) {
    _board = other._board;
    _curr_player = other._curr_player;
    _result = other._result;
    _size = other._size;
}

Board::~Board() = default;

bool Board::isMoveValid(std::string move, PlayerColor player) {
    if (move.size() != 2 && move.size() != 3) return false;

    if (move.front() >= 'J') --move.front();
    int row = move.front() - 'A';
    if (row < 0 || row >= _size) return false;
    int col = std::stoi(move.substr(1)) - 1;
    if (col < 0 || col >= _size) return false;

    if (_board[row][col] != PointColor::Neutral) return false;

    std::vector<std::pair<int, int>> directions = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
    PlayerColor opponent = (player == PlayerColor::Black ? PlayerColor::White : PlayerColor::Black);

    for (auto dir : directions) {
        int nx = row + dir.first;
        int ny = col + dir.second;
        bool found_opponent = false;

        while (nx >= 0 && nx < _size && ny >= 0 && ny < _size) {
            if (_board[nx][ny] == PointColor(opponent))
                found_opponent = true;
            else if (_board[nx][ny] == PointColor(player) && found_opponent)
                return true;
            else
                break;
            nx += dir.first;
            ny += dir.second;
        }
    }
    return false;
}

bool Board::isMoveValid(std::string move) {
    return isMoveValid(move, _curr_player);
}

std::vector<std::string> Board::getValidMoves(PlayerColor player) {
    std::vector<std::string> moves;
    for (int i = 0; i < _size; ++i)
        for (int j = 1; j <= _size; ++j) {
            std::string move = std::string{char('A' + i)} + std::to_string(j);
            if (move.front() >= 'I') ++move.front();
            if (isMoveValid(move, player)) moves.push_back(move);
        }

    return moves;
}

std::vector<std::string> Board::getValidMoves() {
    return getValidMoves(_curr_player);
}

void Board::makeMove(std::string move) {
    if (move.front() >= 'J') --move.front();
    int row = move.front() - 'A';
    int col = std::stoi(move.substr(1)) - 1;

    _board[row][col] = PointColor(_curr_player);

    std::vector<std::pair<int, int>> directions = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
    PlayerColor opponent = (PointColor(_curr_player) == PointColor::Black) ? PlayerColor::White : PlayerColor::Black;

    for (auto dir : directions) {
        int nx = row + dir.first;
        int ny = col + dir.second;
        std::vector<std::pair<int, int>> to_flip;

        while (nx >= 0 && nx < _size && ny >= 0 && ny < _size) {
            if (_board[nx][ny] == PointColor(opponent))
                to_flip.push_back({nx, ny});
            else if (_board[nx][ny] == PointColor(_curr_player)) {
                for (const auto& pos : to_flip) _board[pos.first][pos.second] = PointColor(_curr_player);
                break;
            } else
                break;

            nx += dir.first;
            ny += dir.second;
        }
    }
    _curr_player = opponent;
}

int Board::evaluateBoard(PlayerColor player) {
    int score = 0;

    static std::map<int, std::vector<std::vector<int>>> weights;

    if (weights.find(9) == weights.end()) {
        weights[9] = {{50, -10, 5, 5, 5, 5, -10, 50},       {-10, -20, -1, -1, -1, -1, -20, -10}, {5, -1, 1, 1, 1, 1, -1, 5},
                      {5, -1, 1, 1, 1, 1, -1, 5},           {5, -1, 1, 1, 1, 1, -1, 5},           {5, -1, 1, 1, 1, 1, -1, 5},
                      {-10, -20, -1, -1, -1, -1, -20, -10}, {50, -10, 5, 5, 5, 5, -10, 50}};

        weights[13] = {{100, -20, 10, 5, 5, 5, 5, 5, 5, 10, -20, 100}, {-20, -30, -5, -3, -3, -3, -3, -3, -3, -5, -30, -20},
                       {10, -5, 2, 2, 2, 2, 2, 2, 2, 2, -5, 10},       {5, -3, 2, 1, 1, 1, 1, 1, 1, 2, -3, 5},
                       {5, -3, 2, 1, 1, 1, 1, 1, 1, 2, -3, 5},         {5, -3, 2, 1, 1, 1, 1, 1, 1, 2, -3, 5},
                       {5, -3, 2, 1, 1, 1, 1, 1, 1, 2, -3, 5},         {5, -3, 2, 1, 1, 1, 1, 1, 1, 2, -3, 5},
                       {10, -5, 2, 2, 2, 2, 2, 2, 2, 2, -5, 10},       {-20, -30, -5, -3, -3, -3, -3, -3, -3, -5, -30, -20},
                       {100, -20, 10, 5, 5, 5, 5, 5, 5, 10, -20, 100}};

        weights[19] = {{120, -30, 15, 10, 8, 7, 6, 6, 6, 6, 6, 6, 7, 8, 10, 15, -30, 120},
                       {-30, -40, -10, -8, -7, -6, -5, -5, -5, -5, -5, -5, -6, -7, -8, -10, -40, -30},
                       {15, -10, 5, 4, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 4, 5, -10, 15},
                       {10, -8, 4, 3, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 3, 4, -8, 10},
                       {8, -7, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, -7, 8},
                       {7, -6, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, -6, 7},
                       {6, -5, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, -5, 6},
                       {6, -5, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, -5, 6},
                       {6, -5, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, -5, 6},
                       {6, -5, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, -5, 6},
                       {7, -6, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, -6, 7},
                       {8, -7, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, -7, 8},
                       {10, -8, 4, 3, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 3, 4, -8, 10},
                       {15, -10, 5, 4, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 4, 5, -10, 15},
                       {-30, -40, -10, -8, -7, -6, -5, -5, -5, -5, -5, -5, -6, -7, -8, -10, -40, -30},
                       {120, -30, 15, 10, 8, 7, 6, 6, 6, 6, 6, 6, 7, 8, 10, 15, -30, 120}};
    }

    for (int i = 0; i < _size; ++i)
        for (int j = 0; j < _size; ++j)
            if (_board[i][j] == PointColor(player))
                score += weights[_size][i][j];
            else if (_board[i][j] == (player == PlayerColor::Black ? PointColor::White : PointColor::Black))
                score -= weights[_size][i][j];

    return score;
}

int Board::alphaBeta(int depth, int alpha, int beta, bool maximizingPlayer, PlayerColor player) {
    std::vector<std::string> currentPlayerMoves = getValidMoves(player);
    std::vector<std::string> opponentMoves = getValidMoves(player == PlayerColor::Black ? PlayerColor::White : PlayerColor::Black);

    if (currentPlayerMoves.empty() && opponentMoves.empty()) return evaluateBoard(player) * 1000;

    if (depth == 0) return evaluateBoard(player);

    std::vector<std::string> moves =
        getValidMoves(maximizingPlayer ? player : (player == PlayerColor::Black ? PlayerColor::White : PlayerColor::Black));

    if (moves.empty()) return alphaBeta(depth - 1, alpha, beta, !maximizingPlayer, player);

    if (maximizingPlayer) {
        int max_eval = INT_MIN;
        for (const auto& move : moves) {
            auto temp_board = *this;
            temp_board.makeMove(move);
            int eval = temp_board.alphaBeta(depth - 1, alpha, beta, false, player);
            max_eval = std::max(max_eval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return max_eval;
    } else {
        int min_eval = INT_MAX;
        for (const auto& move : moves) {
            auto temp_board = *this;
            temp_board.makeMove(move);
            int eval = temp_board.alphaBeta(depth - 1, alpha, beta, true, player);
            min_eval = std::min(min_eval, eval);
            alpha = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return min_eval;
    }
}

std::string Board::getBestMove(int depth, PlayerColor player) {
    std::vector<std::string> moves = getValidMoves(player);
    if (moves.empty()) return "none";

    std::string best_move = moves[0];
    int max_eval = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (auto& move : moves) {
        auto temp_board = *this;
        temp_board.makeMove(move);
        int eval = temp_board.alphaBeta(depth - 1, alpha, beta, false, player);
        if (eval > max_eval) {
            max_eval = eval;
            best_move = move;
        }
        alpha = std::max(alpha, eval);
    }

    return best_move;
}

std::string Board::getBestMove(int depth) {
    return getBestMove(depth, _curr_player);
}

std::string Board::getRandomMove(PlayerColor player) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::string> moves = getValidMoves(player);
    if (moves.empty()) return "none";
    std::uniform_int_distribution<> dis(0, moves.size() - 1);
    return moves[dis(gen)];
}

std::string Board::getRandomMove() {
    return getRandomMove(_curr_player);
}

const std::vector<std::vector<std::string>> Board::getPointBoard() const {
    std::vector<std::vector<std::string>> string_board(_size, std::vector<std::string>(_size));
    for (int i = 0; i < _size; ++i)
        for (int j = 0; j < _size; ++j)
            string_board[i][j] = (_board[i][j] == PointColor::White ? "White" : (_board[i][j] == PointColor::Black ? "Black" : "Neutral"));

    return string_board;
}

std::string Board::result() const {
    return _result;
}

void Board::endGame() {
}