#include <ncurses.h>
#include <string>
#include "../format_datetime.cpp"
class DisplayNews
{
    WINDOW *_parent;
    std::string _text;
    std::string _textstatus;
    attr_t _attributes;

public:
    DisplayNews(WINDOW *parent = stdscr)
    {
        _parent = parent;
        _text = "";
        _textstatus = "";
    }

   
    void draw(json& jsonData,int current_element)
    {
        int row = 0;
        for (int i = 0; i < jsonData["articles"].size(); i++) {
            if(i == current_element) {
                attron(A_STANDOUT);
            }
            if(jsonData["articles"][i]["source"]["name"].is_null() || jsonData["articles"][i]["publishedAt"].is_null() || jsonData["articles"][i]["title"].is_null())
                mvprintw(row++, 0, "something missing");
            else 
                mvprintw(row++,0, " ");
                addstr(std::to_string(i).c_str());
                addstr(" ~ ");
                addstr(jsonData["articles"][i]["source"]["name"].get<std::string>().c_str());
                addstr(" ~ ");
                attron(A_BOLD);	
                addstr((format_datetime(jsonData["articles"][i]["publishedAt"].get<std::string>())).c_str());
                attroff(A_BOLD);	
                addstr(" ~ ");
                addstr(jsonData["articles"][i]["title"].get<std::string>().c_str());
               // mvprintw(row++,0, " ");
            if(i == current_element) {
                attroff(A_STANDOUT);
            }
        }  
     }
 };
    
