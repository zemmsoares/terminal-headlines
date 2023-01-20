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
    int current_element_top_visible = 0;
    int current_element_bottom_visible = 0;

    int x_max = getmaxx(stdscr);
    int y_max = getmaxy(stdscr);
    /*Get request from API */
    /*Creates thread to not interfere with main getch*/
    /*runs every x time to get new data from the API*/
    std::string s;
    bool refreshed = true;
    std::thread t1([&s,&current_element,&refreshed]() {
    Request request;
    StatusBar statusbar;
    DisplayNews displaynews;
   
    while (true) {
        clear();
        request.getRequests();
        
        // read json file
        std::ifstream jsonFile("data.json");
        json jsonData;
        jsonFile >> jsonData;
        jsonFile.close();

        //display news
        displaynews.draw(jsonData,current_element,true,0,0,0);

        statusbar.setAttributes(COLOR_PAIR(2));
        statusbar.drawBg();
        statusbar.setText(" Press q to exit ", COLOR_PAIR(1));
        statusbar.setTextStatus(" PRESS Q TO EXIT ", COLOR_PAIR(1));
        statusbar.draw();
        
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
    displaynews.draw(jsonData,current_element,refreshed,0,0,0);
    
    /*Status Bar */
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);

    refresh(); //refresh screen

    curs_set(0); //hide cursor
    
    int key;
    while((key = getch()) != 'q'){
        switch(key){
            case 'j':
                if(current_element < jsonData["articles"].size()-1) {
            current_element++;
            if(current_element >= y_max - 3) {
                if(current_element_bottom_visible < jsonData["articles"].size()-1) {
                    current_element_bottom_visible++;
                }
                if(current_element_top_visible < jsonData["articles"].size()-1) {
                    current_element_top_visible++;
                }
            }
        }                break;
            case 'k':
              if(current_element > 0) {
            current_element--;
            if(current_element < y_max - 3) {
                if(current_element_bottom_visible > 0) {
                    current_element_bottom_visible--;
                }
                if(current_element_top_visible > 0) {
                    current_element_top_visible--;
                }
            }
        }                break;
        case '\n':
            display_modal(current_element,jsonData);
            break;
        case 'o':
            open_link(current_element,jsonData);
        }

    clear();
    
    refreshed = false;

    // read json file
    std::ifstream jsonFile("data.json");
    json jsonData;
    jsonFile >> jsonData;
    jsonFile.close();







    //display news
    displaynews.draw(jsonData,current_element,refreshed,y_max,current_element_bottom_visible,
            current_element_top_visible);
    
    StatusBar statusbar;
    statusbar.setAttributes(COLOR_PAIR(2));
    statusbar.drawBg();
    statusbar.draw();
    statusbar.setAttributes(COLOR_PAIR(4));
    statusbar.setLastUpdate(s);
    statusbar.drawLastUpdate();
   
    /*quit*/
    statusbar.setAttributes(COLOR_PAIR(3));
    statusbar.drawKey(" q ",1);
    statusbar.setAttributes(COLOR_PAIR(4));
    statusbar.drawKey(" quit ",4);
    /*open in browser*/
    statusbar.setAttributes(COLOR_PAIR(3));
    statusbar.drawKey(" o ",11);
    statusbar.setAttributes(COLOR_PAIR(4));
    statusbar.drawKey(" open ",14);
 
    refresh();
    }


    endwin();
    return 0;
}              


