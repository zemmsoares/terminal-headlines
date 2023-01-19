#include <iostream>
#include <fstream>
#include <ncurses.h>
#include "json.hpp"

using json = nlohmann::json;

int getGui()
{
    // initialize ncurses
    initscr();
    start_color();
    noecho();
    cbreak();

    // read json file
    std::ifstream jsonFile("data.json");
    json jsonData;
    jsonFile >> jsonData;
    jsonFile.close();

    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
      // display json data
    int row = 0;
    for (auto& element : jsonData["articles"]) {
        
        if(element["source"]["name"].is_null() )
            mvprintw(row++, 0, "source: null");
        else
            attron(COLOR_PAIR(1));
            attron(A_BOLD);
            mvprintw(row++, 0, (" "+element["source"]["name"].get<std::string>()+" ").c_str());
        if(element["title"].is_null())
            mvprintw(row++, 0, "title: null");
        else 
            attron(COLOR_PAIR(2));
            attroff(A_BOLD);	
            addstr((" - "+ element["title"].get<std::string>()).c_str());
    }
    // wait for user input

    mvprintw(LINES - 1,0, "NORMAL");
    refresh();



    while(true){
        int ch = getch();
        if(ch == 'q'){
            break;
        }
    }

    // cleanup ncurses
    endwin();
    return 0;
}


