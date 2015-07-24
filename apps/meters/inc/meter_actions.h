#ifndef __METER_ACTIONS_H__
#define __METER_ACTIONS_H__

#include "meter_app.h"

#define MAX_ACTION_LENGTH	45
#define MAX_DESCRIPTION_LENGTH	255

typedef int (*action_f)(char *device, char *sub_action, option_flag_t  flags);

typedef struct action_X
{
	char name[MAX_ACTION_LENGTH];
	char description[MAX_DESCRIPTION_LENGTH];
	action_f act;
}action_t;

int meter_read(char *device, char *sub_action, option_flag_t flags);
int meter_info(char *device, char * sub_action, option_flag_t flags);
int meter_test(char *device, char *sub_action, option_flag_t flags);

#endif
