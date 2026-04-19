/*
 * command_control.h
 *
 *  Created on: Apr 15, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include <string.h>
#include <stdlib.h>

typedef enum
{
	UNKNOWN_COMMAND = 0,
	STAT_COMMAND,
	LOG_COMMAND,
	DUMP_COMMAND,
	SETWARN_COMMAND,
	SFD_COMMAND,
	RFE_COMMAND,
	CLF_COMMAND,
	SETRST_COMMAND
}command_e;

command_e command_type(char *command);

#endif /* INC_COMMAND_H_ */
