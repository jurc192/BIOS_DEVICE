/*
    BIOS_device
    main.c

    BIOS_DEVICE main screen w/ main menu
*/
#include "main.h"

int main(void)
{

    /* NCURSES INITIALIZATION */
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);

    mvprintw(LINES - 3, 1, "Press q to exit");
    refresh();


    /* MENU STUFF */
    ITEM **items = (ITEM **) calloc(5, sizeof(ITEM *));
    if (items == NULL) {
        printw("Memory allocation err");
        refresh();
    }
    items[0] = new_item("   UPLOAD A HACK   ", "Upload your program/demo/hack to the hacks database");
    items[1] = new_item("   WRITE A HACK    ", "Write an assembly program/demo");
    items[2] = new_item("       HACKS       ", "Show all the uploaded hacks");
    items[3] = new_item("       ABOUT       ", "Show information about this project");
    items[4] = (ITEM *)NULL;                // Array of items has to be null terminated

    MENU *menu_main = new_menu(items);


    /* MENU WINDOW W/ BORDER STUFF */
    int mw_height = 8;      // Hardcoded - todo: dynamic size
    int mw_width = 23;
    int mw_x = (COLS - mw_width) / 2;
    int mw_y = (LINES - mw_height) / 2;

    WINDOW *menu_window = newwin(mw_height, mw_width, mw_y, mw_x);
    wborder(menu_window, '0', '0', '#', '#', '#', '#', '#', '#');
    // TODO: Check if menuwindow != null

    WINDOW *menu_subw = derwin(menu_window, mw_height-2, mw_width-2, 2, 2);

    menu_opts_off(menu_main, O_SHOWDESC);
    set_menu_mark(menu_main, NULL);
    set_menu_spacing(menu_main, 0, 0, 4);
    set_menu_win(menu_main, menu_window);
    set_menu_sub(menu_main, menu_subw);
    
    keypad(menu_window, TRUE);
    post_menu(menu_main);
    wrefresh(menu_window);      // TO moram narisat


    /* MAIN LOOP */
    ITEM *curr_item = current_item(menu_main);
    print_info(item_description(curr_item));        // Mogoče to?

    int c;
    while( (c = wgetch(menu_window)) != 'q') {

        switch(c) {
            case KEY_DOWN:
                menu_driver(menu_main, REQ_DOWN_ITEM);
                break;

            case KEY_UP:
                menu_driver(menu_main, REQ_UP_ITEM);
                break;

            /* Regular enter; KEY_ENTER works only for the numpad key */
            case 10:
                clear_mainscr(menu_main, menu_window, menu_subw);
                menu_handler(curr_item);
                draw_mainscr(menu_main, menu_window);
                break;
        }

        curr_item = current_item(menu_main);
        print_info(item_description(curr_item));
    }
    

    /* CLEANING UP AND EXITING PROGRAM */
    unpost_menu(menu_main);
    for(int i=0; i<4; i++) {
        free_item(items[i]);
    }
    free_menu(menu_main);
    free(items);    /* Don't forget to free allocated memory */
    endwin();

    return 0;

}


/* Clear main screen - just to be sure */
void clear_mainscr(MENU *menu, WINDOW *mw, WINDOW* msw)
{
    unpost_menu(menu); // Redrawing of menu doesn't work if you don't unpost it first afaik
    wrefresh(msw);
    wclear(mw);
    wrefresh(mw);
    clear();
    refresh();
}


/* Clear screen and redraw main menu */
void draw_mainscr(MENU *menu, WINDOW *mw)
{
    clear();
    refresh();
    post_menu(menu);
    wborder(mw, '0', '0', '#', '#', '#', '#', '#', '#');
    wrefresh(mw);
}

/* Run selected option */
int menu_handler(ITEM *cur)
{
    int ndx = item_index(cur);

    switch(ndx) {
        case 0:
            // UPLOAD A HACK
            print_info("UPLOAD A HACK");
            break;
        case 1:
            // WRITE A HACK
            print_info("WRITE A HACK");
            break;
        case 2:
            // HACKS
            print_info("HACKS!");
            hacks_screen();
            break;
        case 3:
            // ABOUT
            print_info("ABOUT");
            break;
    }

    return 0;
}