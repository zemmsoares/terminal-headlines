void display_modal(int current_element, json jsonData) {
    // create new window and position it in the center of the screen
    int modal_height = 10;
    int modal_width = 30;
    int starty = (LINES - modal_height) / 2;
    int startx = (COLS - modal_width) / 2;
    WINDOW *modal = newwin(modal_height, modal_width, starty, startx);

    // display information
    mvwprintw(modal, 1, 1, "Title: %s", jsonData["articles"][current_element]["title"].get<std::string>().c_str());
    mvwprintw(modal, 2, 1, "Author: %s", jsonData["articles"][current_element]["author"].get<std::string>().c_str());
    mvwprintw(modal, 3, 1, "Source: %s", jsonData["articles"][current_element]["source"]["name"].get<std::string>().c_str());
    mvwprintw(modal, 4, 1, "Published at: %s", jsonData["articles"][current_element]["publishedAt"].get<std::string>().c_str());

    // refresh and wait for user input
    wrefresh(modal);
    getch();

    // clean up
    delwin(modal);
}
