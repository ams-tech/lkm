#include "meter_actions.h"
#include "meter_app.h"
#include "module/meter_app.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include "device_actions.h"

int meter_test(char *device, char *sub_action, option_flag_t flags)
{
	int fd;

	if(device == NULL)
		return -1;

	printf("Device Name: %s\r\n", device);

	fd = device_open(device);
	if (fd < 0)
	{
		printf("Error opening meter\r\n");
		return fd;
	}
	device_close(fd);

	return 0;
}

int meter_read(char *device, char *sub_action, option_flag_t flags)
{

	int fd, retval = 0;
	float value;
	meter_data_t data;

	if(device == NULL)
		return -1;

	fd = device_open(device);
	if (fd < 0)
	{
		return fd;
	}

	if(read_value(fd, &value) != DEVICE_SUCCESS)
	{
		printf("Unable to read value from meter!\r\n");
	}
	else
	{
		printf("Read value %f from meter\r\n", value);
	}

	device_close(fd);
	return retval;
}

