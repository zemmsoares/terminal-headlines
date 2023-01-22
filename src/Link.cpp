#include "json.hpp"

using json = nlohmann::json;

void open_link(int current_element, json newsJson) {
    std::string link = newsJson["articles"][current_element]["url"].get<std::string>();
    std::string command = "xdg-open " + link;
    system(command.c_str());
}
