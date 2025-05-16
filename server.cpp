#include "httplib.h"
#include "json.hpp"
#include "alg.h"

using json = nlohmann::json;

int main() {
    httplib::Server svr;


    svr.Post("/solve", [](const httplib::Request& req, httplib::Response& response) {
        try {
            auto data = json::parse(req.body);

            struct result res;

            double temp = data.at("temp");
            double hum  = data.at("hum");

            std::string method = data.value("method", "mamdani");
            bool useMamdani = (method == "mamdani");

            fuzzy_inference(temp, hum, &res, useMamdani);

            json response_json = {
            {Ai[0], res.phasificationA[0]},
            {Ai[1], res.phasificationA[1]},
            {Ai[2], res.phasificationA[2]},
            {Bi[0], res.phasificationB[0]},
            {Bi[1], res.phasificationB[1]},
            {Bi[2], res.phasificationB[2]},
            {"aggregation: ", {
                res.aggregation[0], res.aggregation[1], res.aggregation[2],
                res.aggregation[3], res.aggregation[4], res.aggregation[5],
                res.aggregation[6], res.aggregation[7], res.aggregation[8]
            }},
            {"y0", res.y0},
            {"Ci:", res.C}
        };


            response.set_content(response_json.dump(4), "application/json");
        } catch (const std::exception& e) {
            response.status = 400;
            response.set_content("Bad Request: " + std::string(e.what()), "text/plain");
        }
    });

    svr.listen("127.0.0.1", 18080);
}
