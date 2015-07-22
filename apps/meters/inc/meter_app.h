#ifndef __METER_APP_H__
#define __METER_APP_H__

#include <stdint.h>
#include "module/meter_app.h"

typedef uint32_t option_flag_t;

typedef void (*option_f)(void);

#define MAX_ARGS	3
#define MAX_OPTIONS	(sizeof(option_flag_t) * 8)

#define MAX_ACTION_LENGTH	45
#define MAX_DESCRIPTION_LENGTH	255

typedef struct option_X
{
	char char_flag;
	char description[MAX_DESCRIPTION_LENGTH];
	option_f pre_call;
}option_t;

#endif

