#include "src/StatusBar.hpp"
#include "src/Request.hpp"
#include "src/DisplayNews.hpp"
#include <ncurses.h>


#include "modal.cpp"
#include "current_time.cpp"
#include "link.cpp"
#include "update_thread.cpp"


#include <iostream>
#include <thread>
#include <chrono>
#include <future>
int main()
{
    initscr();
    noecho();
    cbreak();

    use_default_colors();
    
    /*current selected element for the list*/
    int current_element = 0;

    int x_max = getmaxx(stdscr);
    /*Get request from API */
    /*Creates thread to not interfere with main getch*/
    /*runs every x time to get new data from the API*/
    std::string s;
    std::thread t1([&s]() {
    Request request;
    StatusBar statusbar;
   
    while (true) {

        request.getRequests();
        s = CurrentDate();
        statusbar.setLastUpdate(s);
        statusbar.drawLastUpdate();
        refresh();
        std::this_thread::sleep_for(std::chrono::minutes(5)); 
        refresh();
        }
    });
    t1.detach();

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
    init_pair(3, COLOR_WHITE, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA);

    refresh(); //refresh screen

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
        }

    clear();

    // read json file
    std::ifstream jsonFile("data.json");
    json jsonData;
    jsonFile >> jsonData;
    jsonFile.close();

    //display news
    displaynews.draw(jsonData,current_element);
    
    StatusBar statusbar;
    statusbar.setAttributes(COLOR_PAIR(2));
    statusbar.drawBg();
    statusbar.setText(" Press q to exit ", COLOR_PAIR(1));
    statusbar.setTextStatus(" NO ELEMENT SELECTED ", COLOR_PAIR(1));
    statusbar.draw();
    statusbar.setLastUpdate(s);
    statusbar.drawLastUpdate();

    refresh();
    }


    endwin();
    return 0;
}              


