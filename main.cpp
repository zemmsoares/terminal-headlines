#include <cstdio>
#include <curses.h>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include "./src/Request.hpp"
#include "./src/StatusBar.hpp"
#include "./src/DisplayNews.hpp"
#include "src/Link.cpp"
#include "src/Modal.cpp"
#include "./src/json.hpp"

using json = nlohmann::json;

int main() {
    initscr();
    noecho();
    cbreak();
    use_default_colors();
    curs_set(0);

    /*Status Bar */
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);

    /*current selected element for the list*/
    int current_element = -1;
    /*current top/bot elements for scrolling behaviour*/
    int current_element_top_visible = 0;
    int current_element_bottom_visible = 0;

    int x_max = getmaxx(stdscr);
    int y_max = getmaxy(stdscr);

    Request request;
    request.getRequests();

    std::ifstream jsonFile("./data/data.json");
    json myJson;
    jsonFile >> myJson;
    jsonFile.close(); 

    /*Create thread t1 to not interfere with main getch*/
    /*runs every x time to get new data from the API*/
    std::string last_update_time;
    bool refreshed = true;
    std::thread t1([&myJson,&last_update_time,&current_element,&refreshed]() {
            Request request;
            DisplayNews displaynews;
            StatusBar statusbar;
            while (true) {
            clear();

            /*update json with new data from API*/
            request.getRequests();

            std::ifstream jsonFile("./data/data.json");
            jsonFile >> myJson;
            jsonFile.close();

            /*remove the highlighted selection after refresh*/
            refreshed = true;

            displaynews.draw(myJson, current_element, refreshed, 0, 0, 0);

            /*StatusBar*/
            statusbar.setAttributes(COLOR_PAIR(2));
            statusbar.drawBg();
            statusbar.draw();


            statusbar.setAttributes(COLOR_PAIR(4));
            /*Save time each update is done*/
            last_update_time = CurrentDate();
            statusbar.setLastUpdate(last_update_time);
            statusbar.drawLastUpdate();


            /*quit*/
            statusbar.setAttributes(COLOR_PAIR(3));
            statusbar.drawKey(" q ", 1);
            statusbar.setAttributes(COLOR_PAIR(4));
            statusbar.drawKey(" quit ", 4);
            /*open in browser*/
            statusbar.setAttributes(COLOR_PAIR(3));
            statusbar.drawKey(" o ", 11);
            statusbar.setAttributes(COLOR_PAIR(4));
            statusbar.drawKey(" open in browser ", 14);

            refresh();

            /*Sleep before next update*/
            std::this_thread::sleep_for(std::chrono::minutes(15));
            }
    });
    t1.detach();


    // AFTER getch
    DisplayNews displaynews;
    int key;
    while ((key = getch()) != 'q') {
        switch (key) {
            case 'j':
                if(current_element <= myJson["articles"].size() - 2 || current_element== - 1){
                    current_element++;
                    if (current_element >= 0 && current_element < myJson["articles"].size() - 1) {
                        if (current_element >= y_max - 3) {
                            if (current_element_bottom_visible < myJson["articles"].size() - 1) {
                                current_element_bottom_visible++;
                            }
                            if (current_element_top_visible < myJson["articles"].size() - 1) {
                                current_element_top_visible++;
                            }
                        }
                    }      }
                break;
            case 'k':
                if (current_element >= 1) {
                    current_element--;
                    if (current_element < y_max - 3) {
                        if (current_element_bottom_visible > 0) {
                            current_element_bottom_visible--;
                        }
                        if (current_element_top_visible > 0) {
                            current_element_top_visible--;
                        }
                    }
                }      break;
            case '\n':
                display_modal(current_element, myJson);
                break;
            case 'o':
                open_link(current_element, myJson);
        }
        clear();
        refreshed = false;
        // read json file
        std::ifstream jsonFile("./data/data.json");
        jsonFile >> myJson;
        jsonFile.close(); 
        // display news
        displaynews.draw(myJson, current_element, refreshed, y_max,
                current_element_bottom_visible,
                current_element_top_visible);

        StatusBar statusbar;
        statusbar.setAttributes(COLOR_PAIR(2));
    statusbar.drawBg();
    statusbar.draw();
    statusbar.setAttributes(COLOR_PAIR(4));
    statusbar.setLastUpdate(last_update_time);
    statusbar.drawLastUpdate();

    /*quit*/
    statusbar.setAttributes(COLOR_PAIR(3));
    statusbar.drawKey(" q ", 1);
    statusbar.setAttributes(COLOR_PAIR(4));
    statusbar.drawKey(" quit ", 4);
    /*open in browser*/
    statusbar.setAttributes(COLOR_PAIR(3));
    statusbar.drawKey(" o ", 11);
    statusbar.setAttributes(COLOR_PAIR(4));
    statusbar.drawKey(" open in browser ", 14);

    refresh();
  
  }
  endwin();
  return 0;
}
