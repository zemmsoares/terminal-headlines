#include <ncurses.h>
#include <chrono>
#include <thread>
#include "src/DisplayNews.hpp"
#include "src/Request.hpp"
#include "src/StatusBar.hpp"
#include "src/current_time.cpp"
#include "src/link.cpp"
#include "src/modal.cpp"
#include <mutex>
int main() {
    initscr();
    noecho();
    cbreak();
    use_default_colors();
    curs_set(0); // hide cursor

    /*current selected element for the list*/
    int current_element = 0;
    /*current top/bot elements for scrolling behaviour*/
    int current_element_top_visible = 0;
    int current_element_bottom_visible = 0;

    int x_max = getmaxx(stdscr);
    int y_max = getmaxy(stdscr);

    std::mutex json_mutex;

    /*Grab initial news from API*/
    Request request;
    request.getRequests();

    //std::mutex json_mutex;
    // read json file
    std::ifstream jsonFile("./data/data.json");
    json newsJson;
    jsonFile >> newsJson;
    jsonFile.close();  // Display news from JSON file

    /*Create thread t1 to not interfere with main getch*/
    /*runs every x time to get new data from the API*/
    std::string last_update_time;
    bool refreshed = true;
    std::thread t1([&last_update_time, &current_element, &refreshed, &newsJson, 
            &json_mutex]() {
            Request request;
            StatusBar statusbar;
            DisplayNews displaynews;

            while (true) {
            clear();

            /*update json with new data from API*/
            json_mutex.lock();
            request.getRequests();
            json_mutex.unlock();

            // read json file
            json_mutex.lock();
            std::ifstream jsonFile("./data/data.json");
            jsonFile >> newsJson;
            jsonFile.close();
            json_mutex.unlock();

            /*remove the highlighted selection after refresh*/
            refreshed = true;

            // display news
            displaynews.draw(newsJson, current_element, refreshed, 0, 0, 0);

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

            /*Refresh changes*/
            refresh();

            /*Sleep before next update*/
            std::this_thread::sleep_for(std::chrono::minutes(5));
            }
    });
    t1.detach();

    /*-------------------------------------*/


                       //
  DisplayNews displaynews;
  //displaynews.draw(newsJson, current_element, refreshed, 0, 0, 0);

  /*Status Bar */
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_BLACK, COLOR_BLACK);
  init_pair(3, COLOR_BLACK, COLOR_RED);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);

  refresh(); // refresh screen

  /*-------------------------------------*/
  int key;
  while ((key = getch()) != 'q') {
    switch (key) {
    case 'j':
      if (current_element < newsJson["articles"].size() - 1) {
        current_element++;
        if (current_element >= y_max - 3) {
          if (current_element_bottom_visible <
              newsJson["articles"].size() - 1) {
            current_element_bottom_visible++;
          }
          if (current_element_top_visible < newsJson["articles"].size() - 1) {
            current_element_top_visible++;
          }
        }
      }
      break;
    case 'k':
      if (current_element > 0) {
        current_element--;
        if (current_element < y_max - 3) {
          if (current_element_bottom_visible > 0) {
            current_element_bottom_visible--;
          }
          if (current_element_top_visible > 0) {
            current_element_top_visible--;
          }
        }
      }
      break;
    case '\n':
      display_modal(current_element, newsJson);
      break;
    case 'o':
      open_link(current_element, newsJson);
    }

    clear();

    /**/
    refreshed = false;
json_mutex.lock();


    // read json file
    std::ifstream jsonFile("./data/data.json");
    json newsJson;
    jsonFile >> newsJson;
    jsonFile.close();
json_mutex.unlock();


    // display news
    displaynews.draw(newsJson, current_element, refreshed, y_max,
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
    statusbar.drawKey(" open ", 14);

    refresh();
  }

  endwin();
  return 0;
}
