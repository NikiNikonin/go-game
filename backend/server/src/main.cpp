#include "../../game/include/board.hpp"
#include "../../lib/httplib.h"
#include "../../lib/json.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace httplib;
using json = nlohmann::json;

Board* board = nullptr;

json boardToJson(Board& b) {
    json j;
    j["board"] = b.getBoard();
    if (b.result() != "none") j["result"] = b.result();
    return j;
}

void addCorsHeaders(Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    Server svr;

    svr.Options("/.*", [](const Request& req, Response& res) {
        addCorsHeaders(res);
        res.status = 200;
    });

    svr.Post("/new_game", [](const Request& req, Response& res) {
        addCorsHeaders(res);

        json request;
        try {
            request = json::parse(req.body);
        } catch (const json::parse_error& e) {
            res.status = 400;
            res.set_content("Invalid JSON", "text/plain");
            return;
        }

        if (!request.contains("size")) {
            res.status = 400;
            res.set_content("Missing size", "text/plain");
            return;
        }

        int size = request["size"];
        if (size < 0 || size > 2) {
            res.status = 400;
            res.set_content("Invalid board size", "text/plain");
            return;
        }

        if (board) delete board;
        board = new Board(size);

        res.set_content(boardToJson(*board).dump(), "application/json");
    });

    svr.Post("/move", [](const Request& req, Response& res) {
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
        if (!board->makeMove(move)) {
            res.set_content("false", "application/json");
        } else {
            res.set_content(boardToJson(*board).dump(), "application/json");
        }

        return;
    });

    svr.listen("localhost", 8080);
}