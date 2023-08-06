#include <curses.h>
#include<ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "display_man.h"
#include "system_info.h"


static void display_generate_progress_bar(int max_len, int end_len, char **pb, float ratio);
    
/* display_top 
 * writes texts to the top window of the screen
 */
void display_top(WINDOW *win, sys_info_t *sys_info, char **mem_pb, char **cpu_pb) {
	int row = 0;
	mvwprintw(win, ++row, 2, "OS		   : %s", sys_info->os);
	mvwprintw(win, ++row, 2, "Kernel	   : %s", sys_info->kernel);
	mvwprintw(win, ++row, 2, "Uptime	   : HH:%02ld-MM:%02ld-SS:%02ld", (sys_info->uptime % 86400)/3600, (sys_info->uptime%3600)/ 60, (sys_info->uptime)%60);

	display_generate_progress_bar(50, 10, mem_pb, sys_info->mem_utilisation);
	display_generate_progress_bar(50, 10, cpu_pb, sys_info->cpu_utilisation);
    
    mvwprintw(win, ++row, 2, "CPU		   :");
    wattron(win, COLOR_PAIR(1));
    wprintw(win, "%s", (*cpu_pb));
    wattroff(win, COLOR_PAIR(1));	

    mvwprintw(win, ++row, 2, "MEM		   :");
    wattron(win, COLOR_PAIR(1));
    wprintw(win, "%s", (*mem_pb));
    wattroff(win, COLOR_PAIR(1));

	mvwprintw(win, ++row, 2, "Total Processes  : %lld", sys_info->procs_info.total_processes);
	mvwprintw(win, ++row, 2, "Running Processes: %lld", sys_info->procs_info.running_processes);
	wrefresh(win);
}

/*
 * Display Manager 
 * initializes all the windows and updates them
 * preodically.
 */
void display_sys_win( sys_info_t *sys_info) {
    char *mem_pb = NULL;
    char *cpu_pb = NULL;

    
	int wmax_x = 0;
	initscr();
	noecho();
	cbreak();
	start_color();

	wmax_x= getmaxx(stdscr);
	WINDOW *top_win = newwin(9, wmax_x -1, 0, 0);
	
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    mem_pb = calloc(50 + 10, sizeof(char));
    cpu_pb = calloc(50 + 10, sizeof(char));

	while(1) {
		box(top_win, 0, 0);
        system_get_time(sys_info);
        system_get_processes_info(sys_info);
        system_get_cpu_utilisation(sys_info);
        system_get_mem_utilisation(sys_info);
		display_top(top_win, sys_info, &mem_pb, &cpu_pb);
		wrefresh(top_win);
		refresh();
		sleep(1);
	}
	endwin();
    
    free(mem_pb);
    free(cpu_pb);
    mem_pb = NULL;
    cpu_pb = NULL;
}

/*
 * Generate progress bar returns a string of progress bar
 * for the ratio passed
 */
static void display_generate_progress_bar(int max_len, int end_len, char **pb, float ratio){

    if(!pb || !*pb) {
        fprintf(stderr, "Invalid pb passed!\n");
        return;
    }

    int bars_count = ratio * max_len;
    
    for( int i = 0; i < max_len - 1; i++ )
       (*pb)[i] = (i < bars_count)?'|':' ';

    sprintf((*pb) + max_len - 1, "%.2f/100%s", ratio * 100, "%");
    
}
