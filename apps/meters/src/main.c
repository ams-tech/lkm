#include "module/meter_app.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ERROR_FORMAT()	printf("Execution format: \r\n\t./meters DEVICE_NAME DEVICE_ACTION (SUB_ACTION) (-OPTIONS) \r\nRun 'meters -h' for more info\r\n")

typedef void (*option_f)(void);

typedef uint32_t option_flag_t;
/*Disk space is cheap*/
#define MAX_DESCRIPTION_LENGTH	255
#define MAX_ACTION_LENGTH 45

#define MAX_ARGS	3
#define MAX_OPTIONS	(sizeof(option_flag_t) * 8)


typedef struct option_X
{
	char char_flag;
	char description[MAX_DESCRIPTION_LENGTH];
	option_f pre_call; /* This function is called before anything else is parsed.  i.e. if we see the flag, call it */
}option_t;

typedef struct action_X
{
	char name[MAX_ACTION_LENGTH];
	char description[MAX_DESCRIPTION_LENGTH];
}action_t;

void help_menu(void);
option_flag_t handle_options_arg(char *argv);

action_t actions[] =
{
	{"read", "read the value from the meter"},
};

#define NUM_ACTIONS	(sizeof(actions) / sizeof(action_t))

option_t options[] = 
{
	{'h', "The help menu.  You're reading it right now!", &help_menu},
};

#define NUM_OPTIONS	(sizeof(options)/sizeof(option_t))

_Static_assert(NUM_OPTIONS <= MAX_OPTIONS, 
		"ERROR: Size of options_flag_t is too small.");

void help_menu(void)
{
	int i;

	printf("List of available actions (not necessarily supported on all devices):\r\n");

	for (i = 0; i < NUM_ACTIONS; i++)
	{
		printf("\t%s :\r\n\t\t%s\r\n", 
			actions[i].name, actions[i].description);
	}


	printf("List of avilable options:\r\n");

	for (i = 0; i < NUM_OPTIONS; i++)
	{
		printf("\t-%c :\t%s\r\n",options[i].char_flag, 
			options[i].description);
	}

}

option_flag_t handle_options_arg(char *argv)
{
	int i, j;
	option_flag_t retval = 0;
	if((argv == NULL) || (argv[0] != '-'))
		return 0;

	for( i = 1; i < strlen(argv); i++)
	{
		for( j = 0; j < NUM_OPTIONS; j++)
		{
			if(options[j].char_flag == argv[i])
			{
				retval |= (1 << j);
				break;
			}
		}
		if(j == NUM_OPTIONS)
		{
			printf("WARNING: Option flag %c does not exist\r\n", 
				argv[i]);
		}
	}
	return retval;
}

int main( int argc, char *argv[])
{
	char *my_args[MAX_ARGS];
	int num_args = 0;
	int i, j;
	option_flag_t flags = 0;

	for (i = 0; i < argc; i++)
	{
		if(argv[i] != NULL)
		{
			if(argv[i][0] == '-')
				flags |= handle_options_arg(argv[i]);
			else
			{
				if (num_args >= MAX_ARGS)
				{
					ERROR_FORMAT();
					return -1;
				}
				else
				{
					my_args[num_args++] = argv[i];
				}
			}
		}
	}
	for (i = 0; i < NUM_OPTIONS; i++)
	{
		if(((flags & (1 << i)) != 0) && (options[i].pre_call != NULL))
			options[i].pre_call();
	}
	if(num_args < 2)
	{
		ERROR_FORMAT();
		return -1;
	}

	return 0;
} 
