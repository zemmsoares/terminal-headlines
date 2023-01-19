#include "src/StatusBar.hpp"
#include "src/Request.hpp"
#include "src/News.hpp"
#include <ncurses.h>

int main()
{
    initscr();
    noecho();
    cbreak();
    
    int x_max = getmaxx(stdscr);

    /*Get request from API */
    Request request;
    request.getRequests();

    /*List News */
    News news;
    news.getNews();

    /*Status Bar */
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    
    StatusBar statusbar;
    statusbar.setAttributes(COLOR_PAIR(2));
    statusbar.drawBg();
    statusbar.setText(" Press q to exit ", COLOR_PAIR(1));
    statusbar.draw();

    refresh();

    curs_set(0); //hide cursor


    while(true) {
        int ch = getch();
        if(ch == 'q') {
            break;
        }
        // your other code here
    }

    endwin();
    return 0;
}                                       
