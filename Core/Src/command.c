/*
 * command.c
 *
 *  Created on: Apr 15, 2026
 *      Author: Samkelo Nkabinde
 */

#include "command.h"

command_e get_command(char *command)
{
	if(command == NULL) return UNKNOWN_COMMAND;

	if(strcmp(command, "@Stat&\n") == 0)
		return STAT_COMMAND;

	if(strcmp(command, "@Log&\n") == 0)
		return LOG_COMMAND;

	if(strcmp(command, "@Dump&\n") == 0)
		return DUMP_COMMAND;

	if(strcmp(command, "@SetWarn C=1&\n") == 0 || strcmp(command, "@SetWarn C=0&\n") == 0)
		return SETWARN_COMMAND;

	if(strncmp(command, "@SFD ", strlen("@SFD ")) == 0)
	{
		 int len = strlen(command);
		 if(len >= 2 && strcmp(command + len - 2, "&\n") == 0)
			 return SFD_COMMAND;
	}

	if(strcmp(command, "@RFE&\n") == 0)
		return RFE_COMMAND;

	if(strcmp(command, "@CLF&\n") == 0)
		return CLF_COMMAND;

	if(strncmp(command, "@SetRTC ", strlen("@SetRTC ")) == 0)
	{
		 int len = strlen(command);
		 if(len >= 2 && strcmp(command + len - 2, "&\n") == 0)
			 return SETRST_COMMAND;
	}

	return UNKNOWN_COMMAND;
}
