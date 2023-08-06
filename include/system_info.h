#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

typedef struct {
    long long running_processes;
    long long total_processes;
}procs_info_t;

typedef struct {
    char *os;
    char *kernel;
    long uptime;
    procs_info_t procs_info;
}sys_info_t; 

void system_init_info(sys_info_t **sys_info);
void system_dinit_info(sys_info_t **sys_info);
void system_get_info(sys_info_t *sys_info);
void system_get_time(sys_info_t *sys_info);
void system_get_processes_info(sys_info_t *sys_info);

#endif

