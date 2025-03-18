#include <iostream>
#include "../../lib/httplib.h"

int main() {
    httplib::Server svr;

    // CORS Middleware (добавляем заголовки ко всем ответам)
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type, Authorization"}
    });

    // GET-запрос (проверка связи с сервером)
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello", "text/plain");
    });

    // OPTIONS-запрос (чтобы preflight-запросы тоже работали)
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        res.status = 204; // No Content
    });

    std::cout << "Server is running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}
