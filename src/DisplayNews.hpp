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

   
    void draw(json& jsonData,int current_element,bool refreshed,int max,
            int current_element_bottom_visible, int current_element_top_visible)
    {
        int row = 0;
        //Number of rows displayed based on scree max rows
        //redo this with ternary or something more readable
        int size = 0;
        if(jsonData["articles"].size() >= max){
            size = max - 3;
        }else if(jsonData["articles"].size() == max-1){
            size = max - 3;
        }else if(jsonData["articles"].size() == max-2){
            size = max - 1;
        }else{
            size = jsonData["articles"].size();
        }
       

        for (int i = current_element_top_visible; i < jsonData["articles"].size(); i++) {
            if(i == current_element) {
                /* row highlight removed after data refresh */
                if(refreshed){
                    attroff(A_STANDOUT);
                }else{
                    attron(A_STANDOUT);
                }
            }
            if(jsonData["articles"][i]["source"]["name"].is_null() || jsonData["articles"][i]["publishedAt"].is_null() || jsonData["articles"][i]["title"].is_null())
                mvprintw(row++, 0, "something missing");
            else 
                mvprintw(row++,0, " ");
                addstr(std::to_string(i).c_str());
              
                //debug
               addstr(" ~ "); 
                addstr(std::to_string(jsonData["articles"].size()).c_str());

                addstr(" ~ "); 
                addstr(std::to_string(max).c_str());
                
                addstr(" ~ "); 
                addstr(std::to_string(current_element).c_str());

                addstr(" b: "); 
                addstr(std::to_string(current_element_bottom_visible).c_str());

                addstr(" t: "); 
                addstr(std::to_string(current_element_top_visible).c_str());
                /*if int number only has one decimal*/
                /*add a space to even the cols*/
                if(i<10){
                    addstr(" ");
                }
                addstr(" * ");
                /*source name*/
                /*limit char to x chars*/
                std::string sourceName = jsonData["articles"][i]["source"]["name"].get<std::string>();
                if(sourceName.length() > 5) {
                    sourceName = sourceName.substr(0, 5) + "...";
                }
                addstr(sourceName.c_str());
                addstr(" ~ ");
                attron(A_BOLD);
                addstr((format_datetime(jsonData["articles"][i]["publishedAt"].get<std::string>())).c_str());
                attroff(A_BOLD);	
                addstr(" | ");
                addstr(jsonData["articles"][i]["title"].get<std::string>().c_str());
               // mvprintw(row++,0, " ");
            if(i == current_element) {
                attroff(A_STANDOUT);
            }
        } 
        standend();
     }
 };
    
