#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "lnx_parser.h"
#include "system_info.h"

void system_init_info(sys_info_t **sys_info) {
    *sys_info = calloc(1, sizeof(sys_info_t));
    (*sys_info)->os = calloc(1, 1024);
    (*sys_info)->kernel = calloc(1, 1024);
    (*sys_info)->uptime = 0;
}

void system_dinit_info(sys_info_t **sys_info) {

    // free resources
    free((*sys_info)->os);
    (*sys_info)->os = NULL;
    free((*sys_info)->kernel);
    (*sys_info)->kernel = NULL;
    free(*sys_info);
    *sys_info = NULL;
}

void system_get_info(sys_info_t *sys_info) {

    if(!sys_info) {
        fprintf(stderr, "sys_info_t not initialized! Please call system_init_info before callind %s\n", __func__);
        return;
    }

	lnx_parse_os_name(sys_info->os, 1024);
	lnx_parse_kernel_version(sys_info->kernel, 1024);
    system_get_time(sys_info);

}

void system_get_time(sys_info_t *sys_info){
    sys_info->uptime = lnx_parse_uptime();
}

void system_get_processes_info(sys_info_t *sys_info){
    lnx_parse_total_processes(&sys_info->procs_info);
}
