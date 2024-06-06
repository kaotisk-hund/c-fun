#include <ncurses.h>

int main(void)
{
	initscr();
	move(10,10);
	getch();
	WINDOW *win = newwin(10,20,3,3);
	refresh();
	box(win,0,0);
	mvwprintw(win,1,1,"hi");
	wrefresh(win);
	getch();

	endwin();
	return 0;

}
