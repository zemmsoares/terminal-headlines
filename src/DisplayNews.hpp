#include <ncurses.h>
#include <string>
#include "./json.hpp"
#include "./Time.cpp"

using json = nlohmann::json;

class DisplayNews {
    WINDOW *_parent;
    std::string _text;
    std::string _textstatus;
    attr_t _attributes;

    public:
    DisplayNews(WINDOW *parent = stdscr) {
        _parent = parent;
        _text = "";
        _textstatus = "";
    }

    void draw(json &newsJson, int current_element, bool refreshed, int max,
            int current_element_bottom_visible,
            int current_element_top_visible) {
        int row = 0;

        for (int i = current_element_top_visible; i < newsJson["articles"].size();
                i++) {
            if (i == current_element) {
                /* row highlight removed after data refresh */
                if (refreshed) {
                    attroff(A_STANDOUT);
                } else {
                    attron(A_STANDOUT);
                }
            }
            if (newsJson["articles"][i]["source"]["name"].is_null() ||
                    newsJson["articles"][i]["publishedAt"].is_null() ||
                    newsJson["articles"][i]["title"].is_null())
                mvprintw(row++, 0, "something missing");
            else
                mvprintw(row++, 0, " ");
            addstr(std::to_string(i).c_str());

      /*if int number only has one decimal*/
      /*add a space to even the cols*/
      if (i < 10) {
        addstr(" ");
      }
      addstr(" * ");
      /*source name*/
      /*limit char to x chars*/
      std::string sourceName =
          newsJson["articles"][i]["source"]["name"].get<std::string>();
      if (sourceName.length() > 5) {
        sourceName = sourceName.substr(0, 5) + "...";
      }
      addstr(sourceName.c_str());
      addstr(" ~ ");
      attron(A_BOLD);
      addstr((format_datetime(
                  newsJson["articles"][i]["publishedAt"].get<std::string>()))
                 .c_str());
      attroff(A_BOLD);
      addstr(" | ");
      addstr(newsJson["articles"][i]["title"].get<std::string>().c_str());
      // mvprintw(row++,0, " ");
      if (i == current_element) {
        attroff(A_STANDOUT);
      }
    }
    standend();
  }
};
