#include <stdio.h>
#include <stdlib.h>
#include "lnx_parser.h"
#include "system_info.h"
#include "display_man.h"

int main() {
    sys_info_t *sys_info = NULL; 
    system_init_info(&sys_info);
    system_get_info(sys_info);
	display_sys_win(sys_info);
    system_dinit_info(&sys_info);
	return 0;
}
