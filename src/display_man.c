#include<ncurses.h>
#include <unistd.h>
#include "display_man.h"
#include "system_info.h"

void display_top(WINDOW *win, sys_info_t *sys_info) {
	int row = 0;
	mvwprintw(win, ++row, 2, "OS		   : %s", sys_info->os);
	mvwprintw(win, ++row, 2, "Kernel	   : %s", sys_info->kernel);
	mvwprintw(win, ++row, 2, "Uptime	   : HH:%02ld-MM:%02ld-SS:%02ld", (sys_info->uptime % 86400)/3600, (sys_info->uptime%3600)/ 60, (sys_info->uptime)%60);
	mvwprintw(win, ++row, 2, "CPU		   :");
	mvwprintw(win, ++row, 2, "MEM		   :");
	mvwprintw(win, ++row, 2, "Total Processes  : %lld", sys_info->procs_info.total_processes);
	mvwprintw(win, ++row, 2, "Running Processes: %lld", sys_info->procs_info.running_processes);
	wrefresh(win);
}

void display_sys_win( sys_info_t *sys_info) {
	int wmax_x = 0;
	initscr();
	noecho();
	cbreak();
	start_color();

	wmax_x= getmaxx(stdscr);
	WINDOW *top_win = newwin(9, wmax_x -1, 0, 0);
	
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);

	while(1) {
		box(top_win, 0, 0);
        system_get_time(sys_info);
        system_get_processes_info(sys_info);
		display_top(top_win, sys_info);
		wrefresh(top_win);
		refresh();
		sleep(1);
	}
	endwin();
}
