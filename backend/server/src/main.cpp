#include "../../game/include/board.hpp"
#include "../../lib/httplib.h"
#include "../../lib/json.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace httplib;
using json = nlohmann::json;

Board* board = nullptr;
int *first_bot_diff = nullptr, *second_bot_diff = nullptr;

json boardToJson(Board& b) {
    json j;
    j["board"] = b.getPointBoard();
    if (b.result() != "none") j["result"] = b.result();
    return j;
}

void addCorsHeaders(Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    res.set_header("Access-Control-Expose-Headers", "*");
}

int main() {
    Server svr;

    svr.Options("/.*", [](const Request& req, Response& res) {
        addCorsHeaders(res);
        res.status = 200;
    });

    svr.Post("/new-game", [](const Request& req, Response& res) {
        addCorsHeaders(res);

        json request;
        try {
            request = json::parse(req.body);
        } catch (const json::parse_error& e) {
            res.status = 400;
            res.set_content("Invalid JSON", "text/plain");
            return;
        }

        if (!request.contains("size") || !request.contains("_1st_bot_difficulty") || !request.contains("_2st_bot_difficulty")) {
            res.status = 400;
            res.set_content("Missing parameters", "text/plain");
            return;
        }

        int size = request["size"];
        if (size < 0 || size > 2) {
            res.status = 400;
            res.set_content("Invalid board size", "text/plain");
            return;
        }

        if (board) delete board;
        if (first_bot_diff) delete first_bot_diff;
        if (second_bot_diff) delete second_bot_diff;
        board = new Board(size);
        first_bot_diff = new int(request["_1st_bot_difficulty"]);
        second_bot_diff = new int(request["_2st_bot_difficulty"]);

        res.set_content(boardToJson(*board).dump(), "application/json");
    });

    svr.Post("/player-move", [](const Request& req, Response& res) {
        addCorsHeaders(res);
        if (!board) {
            res.status = 400;
            res.set_content("No active game", "text/plain");
            return;
        }

        json request = json::parse(req.body);
        if (!request.contains("move")) {
            res.status = 400;
            res.set_content("Missing 'move' field", "text/plain");
            return;
        }

        std::string move = request["move"];
        if (!board->isMoveValid(move)) {
            res.set_content("false", "application/json");
        } else {
            board->makeMove(move);
            res.set_content(boardToJson(*board).dump(), "application/json");
        }

        return;
    });

    svr.Post("/bot-move", [](const Request& req, Response& res) {
        addCorsHeaders(res);

        if (!board) {
            res.status = 400;
            res.set_content("No active game", "text/plain");
            return;
        }
        try {
            json request = json::parse(req.body);
            int difficulty = (board->_curr_player == PlayerColor::White ? *first_bot_diff : *second_bot_diff);

            std::string bot_move;
            switch (difficulty) {
            case 1:
                bot_move = board->getRandomMove();
                break;
            case 2:
                bot_move = board->getBestMove(3);
                break;
            case 3:
                bot_move = board->getBestMove(5);
                break;
            default:
                bot_move = board->getRandomMove();
            }

            if (bot_move != "none") board->makeMove(bot_move);

            json response = boardToJson(*board);

            res.set_content(response.dump(), "application/json");
        } catch (const json::parse_error& e) {
            res.status = 400;
            res.set_content("Invalid request: " + std::string(e.what()), "text/plain");
        }
    });

    svr.Post("/valid-moves", [](const auto& req, auto& res) {
        addCorsHeaders(res);
        json request = json::parse(req.body);

        auto moves = board->getValidMoves();

        json response;
        response["moves"] = moves;
        res.set_content(response.dump(), "application/json");
    });

    svr.listen("localhost", 8080);
}