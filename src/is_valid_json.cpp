#include "./json.hpp"

using json = nlohmann::json;

[[nodiscard]] bool is_valid_json(const std::string &json_data) {
    try {
        json::parse(json_data);
    } catch (json::parse_error &e) {
        return false;
    }
    return true;
}

