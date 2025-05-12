#include "httplib.h"        // Подключаем httplib (https://github.com/yhirose/cpp-httplib)
#include "json.hpp"         // Подключаем nlohmann/json (https://github.com/nlohmann/json)
#include "alg.h"            // Подключаем ваш алгоритм

using json = nlohmann::json;

int main() {
    httplib::Server svr;

    // Простой GET-запрос
    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("Hello, world!", "text/plain");
    });

    // POST-запрос: вход — JSON {"temp": ..., "hum": ..., "method": "mamdani"/"larsen"}
    svr.Post("/solve", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto data = json::parse(req.body);

            double temp = data.at("temp");
            double hum  = data.at("hum");
            std::string method = data.value("method", "mamdani");

            bool useMamdani = (method == "mamdani");

            double result = fuzzy_inference(temp, hum, useMamdani);

            json response_json = {
                {"result", result}
            };
            res.set_content(response_json.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content("Bad Request: " + std::string(e.what()), "text/plain");
        }
    });

    svr.listen("127.0.0.1", 18080);
}
