#ifndef LNX_PARSER_H
#define LNX_PARSER_H

#include <stdlib.h>
#include "system_info.h"

void lnx_parse_os_name(char *os_name, ssize_t maxlen);
void lnx_parse_kernel_version(char *kernel_version, ssize_t maxlen);
long lnx_parse_uptime();
void lnx_parse_total_processes(procs_info_t *proc_info);
void lnx_parse_total_processes(procs_info_t *proc_info);
float lnx_parse_mem_stat();
float lnx_parse_cpu_stat();

#endif
