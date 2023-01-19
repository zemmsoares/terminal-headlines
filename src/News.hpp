#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "json.hpp"

#include <ctime>
#include <sstream>

#include <ncurses.h>

using json = nlohmann::json;

class News
{ 
    std::string readBuffer;
    json jsonData;
    
public:
   
    News()
    {

    }

    
void handle_input() {
    initscr();
    noecho();
    int current_element = 0;
    int key;
    while((key = getch()) != 'q'){
        switch(key){
            case 'h':
                if(current_element > 0) current_element--;
                break;
            case 'j':
                if(current_element < jsonData["articles"].size()-1) current_element++;
                break;
            // 'k' and 'l' cases
        }
        // display jsonData["articles"][current_element]
        // ...
    }
    endwin();
}
    
    std::string format_datetime(std::string publishedAt) {
        std::tm tm = {};
        std::istringstream ss(publishedAt);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &tm);
        return buffer;
    }


    void getNews()
    {    
    // read json file
    std::ifstream jsonFile("data.json");
    json jsonData;
    jsonFile >> jsonData;
    jsonFile.close();


    int row = 0;
    int count = 0;
    for (auto& element : jsonData["articles"]) {
        
        if(element["source"]["name"].is_null() || element["publishedAt"].is_null() || element["title"].is_null())
            mvprintw(row++, 0, "something missing");
        else      
            addstr(std::to_string(count).c_str());
            addstr(" ~ ");
            addstr(element["source"]["name"].get<std::string>().c_str());
            addstr(" ~ ");
            attron(A_BOLD);	
            addstr((format_datetime(element["publishedAt"].get<std::string>())).c_str());
            attroff(A_BOLD);	
            addstr(" ~ ");
            addstr(element["title"].get<std::string>().c_str());
            mvprintw(row++,0, " ");
        count++;
        }
     }
};
