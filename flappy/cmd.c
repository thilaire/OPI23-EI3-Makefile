#include "cmd.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CMD_LENGHT	12

int
get_command(void) {

  char	cmd[MAX_CMD_LENGHT];

	fgets(cmd, MAX_CMD_LENGHT, stdin);

	for(int i = 0; cmd[i] != '\0'; ++i)
		cmd[i] = toupper(cmd[i]);

	if(*cmd != '\n') {
		if(memcmp(cmd, "UP", 2) == 0) {
			return (int)(cmd[3] - '0');
		}
		if(memcmp(cmd, "DOWN", 4) == 0) {
			return -(int)(cmd[5] - '0');
		}
		if(memcmp(cmd, "HELP", 4) == 0) {
			return CMDHELP;
		}
		if(memcmp(cmd, "EXIT", 4) == 0) {
			return CMDEXIT;
		}
	}

	return -1;
}
