/*
 * command.c
 *
 *  Created on: Apr 15, 2026
 *      Author: Samkelo Nkabinde
 */

#include "command.h"

command_e command_type(char *command)
{
	if(command == NULL) return UNKNOWN_COMMAND;

	if(strcmp(command, "@Stat&") == 0)
		return STAT_COMMAND;

	if(strcmp(command, "@Log&") == 0)
		return LOG_COMMAND;

	if(strcmp(command, "@Dump&") == 0)
		return DUMP_COMMAND;

	if (strncmp(command, "@SetWarn ", 9) == 0)
	{
	    int len = strlen(command);
	    if (len >= 4 && command[len - 1] == '&')
	        return SETWARN_COMMAND;
	}

	if(strncmp(command, "@SFD ", strlen("@SFD ")) == 0)
	{
		 int len = strlen(command);
		 if(len >= 2 && strcmp(command + len - 1, "&") == 0)
			 return SFD_COMMAND;
	}

	if(strcmp(command, "@RFE&") == 0)
		return RFE_COMMAND;

	if(strcmp(command, "@CLF&") == 0)
		return CLF_COMMAND;

	if(strncmp(command, "@SetRTC ", strlen("@SetRTC ")) == 0)
	{
		 int len = strlen(command);
		 if(len >= 2 && strcmp(command + len - 1, "&") == 0)
			 return SETRST_COMMAND;
	}

	return UNKNOWN_COMMAND;
}
