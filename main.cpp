#include "src/StatusBar.hpp"
#include "src/Request.hpp"
#include "src/DisplayNews.hpp"
#include <ncurses.h>


#include "modal.cpp"
#include "current_time.cpp"
#include "link.cpp"


#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    initscr();
    noecho();
    cbreak();
    
    /*current selected element for the list*/
    int current_element = 0;

    int x_max = getmaxx(stdscr);

    /*Get request from API */
    Request request;
    request.getRequests();

    // read json file
    std::ifstream jsonFile("data.json");
    json jsonData;
    jsonFile >> jsonData;
    jsonFile.close();

    // Display news from JSON file
    DisplayNews displaynews;
    displaynews.draw(jsonData,current_element);
    
    /*Status Bar */
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    
    StatusBar statusbar;
    statusbar.setAttributes(COLOR_PAIR(2));
    statusbar.drawBg();
    statusbar.setText(" Press q to exit ", COLOR_PAIR(1));
    statusbar.setTextStatus(" NO ELEMENT SELECTED ", COLOR_PAIR(1));
    statusbar.draw();
    statusbar.drawTextEnd();

    refresh();

    curs_set(0); //hide cursor


    
    int key;
    while((key = getch()) != 'q'){
        switch(key){
            case 'j':
                if(current_element < jsonData["articles"].size()-1) current_element++;
                break;
            case 'k':
                if(current_element > 0) current_element--;
                break;
        case '\n':
            display_modal(current_element,jsonData);
            break;
        case 'o':
            open_link(current_element,jsonData);

            // 'k' and 'l' cases
        }

        // Clear the screen before redrawing the elements
        clear();

        
    // read json file
    std::ifstream jsonFile("data.json");
    json jsonData;
    jsonFile >> jsonData;
    jsonFile.close();


    displaynews.draw(jsonData,current_element);
    
std::string s = CurrentDate();    

    statusbar.setAttributes(COLOR_PAIR(2));
    statusbar.drawBg();
    statusbar.setText(" Press q to exit ", COLOR_PAIR(1));
    statusbar.setTextStatus(std::to_string(current_element));
    statusbar.draw();
    statusbar.setTextTime(s);
    statusbar.drawTextEnd();

        refresh();
    }



    endwin();
    return 0;
}              


