#include "httplib.h"
#include "json.hpp"
#include "alg.h"

using json = nlohmann::json;

int main() {
    httplib::Server svr;


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
