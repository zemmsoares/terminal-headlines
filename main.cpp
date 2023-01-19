#include "src/StatusBar.hpp"
#include "src/Request.hpp"
//#include "src/News.hpp"
#include <ncurses.h>

#include "format_datetime.cpp"
#include "modal.cpp"

int main()
{
    initscr();
    noecho();
    cbreak();

    int x_max = getmaxx(stdscr);
    std::string format_datetime(std::string publishedAt);

    int modal_height = 100;
    int modal_width = 300;
    int starty = (LINES - modal_height)/2;
    int startx = (COLS - modal_height)/2;
    WINDOW *modal = newwin(modal_height, modal_width, starty, startx);
    
    /*Get request from API */
    Request request;
    request.getRequests();


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



    int current_element = 0;
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

            // 'k' and 'l' cases
        }

        // Clear the screen before redrawing the elements
        clear();

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
    statusbar.setAttributes(COLOR_PAIR(2));
    statusbar.drawBg();
    statusbar.setText(" Press q to exit ", COLOR_PAIR(1));
    statusbar.setTextStatus(std::to_string(current_element));
    statusbar.draw();
    statusbar.drawTextEnd();

        refresh();
    }



    endwin();
    return 0;
}              


