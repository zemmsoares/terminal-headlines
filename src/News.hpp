#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "json.hpp"

#include <ctime>
#include <sstream>

using json = nlohmann::json;

class News
{ 
    std::string readBuffer;
    json jsonData;
    
public:
   
    News()
    {

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

   // init_pair(1, COLOR_WHITE, COLOR_RED);
  //  init_pair(2, COLOR_WHITE, COLOR_BLACK);
      // display json data
    int row = 0;
    int count = 0;
    for (auto& element : jsonData["articles"]) {
        
        if(element["source"]["name"].is_null() )
            mvprintw(row++, 0, "source: null");
        else
           // attron(COLOR_PAIR(1));
            attron(A_BOLD);
        
        if(element["publishedAt"].is_null())
            mvprintw(row++, 0, "publishedAt: null");
        else 
           // attron(COLOR_PAIR(2));
            attroff(A_BOLD);	
            
            addstr((" - "+ format_datetime(element["publishedAt"].get<std::string>())).c_str());           mvprintw(row++, 0, (" "+element["source"]["name"].get<std::string>()+" ").c_str());
        
        if(element["title"].is_null())
            mvprintw(row++, 0, "title: null");
        else 
           // attron(COLOR_PAIR(2));
            attroff(A_BOLD);	
            addstr((" - "+ element["title"].get<std::string>() + " t: " + std::to_string(count)).c_str());
        count++;
        }
        }
};
