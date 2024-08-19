#include <cornui/js/request.h>

namespace cornui {
    Request::Request() = default;

    bool Request::use(const std::string& name, std::function<nlohmann::json(nlohmann::json)> callback) {
        if (this->callbacks_.find(name) != this->callbacks_.end()) {
            return false;
        }
        this->callbacks_[name] = std::move(callback);
        return true;
    }

    nlohmann::json Request::get(const std::string& name, nlohmann::json data) {
        if (this->callbacks_.find(name) == this->callbacks_.end()) {
            return nlohmann::json::object({
                { "code", 404 },
                { "message", "error" },
                { "data", nlohmann::json::object() }
            });
        }
        return this->callbacks_[name](std::move(data));
    }
}
