#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "lnx_parser.h"

#define PROC_DIR "/proc/"
#define OS_NAME_FILENAME "/etc/os-release"
#define KERNEL_VERSION_FILENAME "/version"
#define OS_UPTIME_FILENME "/uptime"
#define CPU_STAT_FILENME "/stat"
#define MEM_STAT_FILENME "/meminfo"


int skip_len_until(char *line, const char s){
    int len = 0;
    char *ptr = line;
    while(*ptr != s){
        len++;
        ptr++;
    }
    return len - 1;
}

int skip_len_while(char *line, const char s){
    int len = 0;
    char *ptr = line;
    while(*ptr == s){
        len++;
        ptr++;
    }
    return len + 1;
}

void lnx_parse_os_name(char *os_name, ssize_t maxlen) {
	FILE *fp = NULL;
	ssize_t nread;
	size_t len = 0;
	char *line = NULL;
	const char *key = "PRETTY_NAME";

	if(!os_name)
		return;

	fp = fopen(OS_NAME_FILENAME, "r");
	if (!fp) {
		fprintf(stderr, "error while opening the file %s error %d\n", OS_NAME_FILENAME , errno);
		return;
	}

	while ((nread = getline(&line, &len, fp)) != -1) {
		if(line && (strncmp(line, key, strlen(key)) == 0)) {
			strcpy(os_name, line + strlen(key) + 1);
			os_name[strlen(os_name) - 1] = '\0';
			goto ERROR;
		}
	}
ERROR:
	free(line);
	fclose(fp);
	line = NULL;
}

void lnx_parse_kernel_version(char *kernel_version, ssize_t maxlen) {
	FILE *fp = NULL;
	ssize_t nread;
	size_t len = 0;
	char *line = NULL;

	if(!kernel_version)
		return;

	fp = fopen(PROC_DIR KERNEL_VERSION_FILENAME, "r");
	if (!fp) {
		fprintf(stderr, "error while opening the file %s error %d\n", PROC_DIR KERNEL_VERSION_FILENAME , errno);
		return;
	}
    //Linux version 5

	while ((nread = getline(&line, &len, fp)) != -1) {
		if(line) {
            int len = 0;
            len = skip_len_until(line, ' ');
            len += skip_len_while(line + len + 1, ' ');
            len += skip_len_until(line + len, ' ');
            len += skip_len_while(line + len + 1, ' ');
			
            strcpy(kernel_version, line + len );

            len = skip_len_until(line + len, ' ');
			kernel_version[len + 1] = '\0';
			goto ERROR;
		}
	}
ERROR:
	free(line);
	fclose(fp);
	line = NULL;
}

long lnx_parse_uptime(){
	FILE *fp = NULL;
	ssize_t nread;
	size_t len = 0;
	char *line = NULL;
    char *uptime = NULL, *endptr = NULL;
    uptime = calloc(1, 1024);
    long val = 0;

	if(!uptime)
		return val;

	fp = fopen(PROC_DIR OS_UPTIME_FILENME, "r");
	if (!fp) {
		fprintf(stderr, "error while opening the file %s error %d\n", PROC_DIR OS_UPTIME_FILENME  , errno);
		return val;
	}
    //Linux version 5

	while ((nread = getline(&line, &len, fp)) != -1) {
		if(line) {
            int len = 0;
            strcpy(uptime, line);
            len = skip_len_until(line, ' ');
			uptime[len + 1] = '\0';
            val = strtol(uptime, &endptr, 10);
			goto ERROR;
		}
	}
ERROR:
	free(line);
	fclose(fp);
	line = NULL;
    return val;
}

void lnx_parse_total_processes(procs_info_t *proc_info){
	FILE *fp = NULL;
	ssize_t nread;
	size_t len = 0;
	char *line = NULL;
	const char *key_1 = "processes";
	const char *key_2 = "procs_running";
    char *tmp_str = NULL, *endptr;
    tmp_str = calloc(1, 1024);

	if(!proc_info)
		return;

	fp = fopen(PROC_DIR CPU_STAT_FILENME, "r");
	if (!fp) {
		fprintf(stderr, "error while opening the file %s error %d\n", PROC_DIR CPU_STAT_FILENME, errno);
		return;
	}

	while ((nread = getline(&line, &len, fp)) != -1) {
		if(line && (strncmp(line, key_1, strlen(key_1)) == 0)) {
			strcpy(tmp_str, line + strlen(key_1) + 1);
			tmp_str[strlen(tmp_str) - 1] = '\0';
            proc_info->total_processes = strtol(tmp_str, &endptr, 10);

		}else if(line && (strncmp(line, key_2, strlen(key_2)) == 0)){
			strcpy(tmp_str, line + strlen(key_2) + 1);
			tmp_str[strlen(tmp_str) - 1] = '\0';
            proc_info->running_processes = strtol(tmp_str, &endptr, 10);
			goto ERROR;
        }
	}
ERROR:
	free(line);
	fclose(fp);
	line = NULL;
    free(tmp_str);
    tmp_str = NULL;
}

void lnx_parse_runing_process(){
	printf("runing processes");
}

void lnx_parse_mem_stat(){
	printf("mem stat");
}

void lnx_parse_cpu_stat(){
	printf("cpu stat");
}
