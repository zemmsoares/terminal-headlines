void open_link(int current_element, json jsonData) {
    std::string link = jsonData["articles"][current_element]["url"].get<std::string>();
    std::string command = "xdg-open " + link;
    system(command.c_str());
}
