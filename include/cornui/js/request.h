#pragma once

#include <functional>
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace cornui {
    class Request {
    public:
        Request();

        /**
         * @brief Register a callback for a request.
         * @param name The name of the request.
         * @param callback The callback to use.
         * @return True if the callback was registered, false if the name was already used.
         *
         * The callback should take a json object as input and return a json object as output. The output should follow
         * the RESTful API standard.
         *
         * e.g.
         * ```json
         * {
         *    "code": 200,
         *    "message": "OK",
         *    "data": {
         *        "key": "value"
         *        ...
         *    }
         * }
         * ```
         *
         * Note that this format is not enforced, but it is recommended.
         */
        bool use(const std::string& name, std::function<nlohmann::json(nlohmann::json)> callback);

        /**
         * @brief Get the data from a request.
         * @param name The name of the request.
         * @param data The data to use.
         * @return The data from the request.
         */
        nlohmann::json get(const std::string& name, nlohmann::json data);

    private:
        std::unordered_map<std::string, std::function<nlohmann::json(nlohmann::json)>> callbacks_;
    };
}
