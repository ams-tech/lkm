#include "meter_actions.h"
#include "module/meter_app.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

/* returns a negative error code or an fd */
static int meter_open(char *device)
{
	int retval;
	char magic;

	retval = open(device, O_RDWR);

	if(retval < 0)
	{
		printf("Error opening the device!\r\n");
		return retval;
	}

	ioctl(retval, IOCTL_GET_MAGIC, &magic);

	if(magic != METER_MAGIC_NUMBER)
	{
		printf("Error: device doesn't appear to be a meter device\r\n");
		close(retval);
		return -EINVAL;
	}
}

int meter_read(char *device, char *sub_action, option_flag_t flags)
{
	printf("I'm in the meter read function!\r\n");
	return 0;
}

/*
	Print the info from the device driver.
*/
int meter_info(char *device, char *sub_action, option_flag_t flags)
{
	

}
