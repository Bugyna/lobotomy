#include <ncurses.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

#include "util.h"


void tui_init()
{
	initscr();
	raw();
	cbreak();
	noecho();

	// start_color();
	// init_pair(1, COLOR_RED, COLOR_BLACK);
	// init_pair(2, COLOR_BLACK, COLOR_RED);
	keypad(stdscr, TRUE);
	// attron(COLOR_PAIR(1));
	// attron(COLOR_PAIR(2));
	// curs_set(FALSE);
	box(stdscr, '|', '-');
	refresh();
	
	// initscr();
	// box(stdscr, '|', '-');
	// wborder(stdscr, '|', '|', '-', '-', 1, 1, 1, 1);
	// refresh();
	// raw();
	// cbreak();
	// keypad(stdscr, TRUE);
	// noecho();
}


void tui_exit(int n)
{
	noraw();
	refresh();
	endwin();
	_real_exit(n);
}