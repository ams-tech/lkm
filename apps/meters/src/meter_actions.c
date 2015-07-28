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

	retval = open(device, O_RDONLY);

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
	else
		PRINT_DEBUG("Successfully opened meter and read the magic num");
	
	return retval;
}

static int meter_close(int fd)
{
	if(fd < 0)
		return -EBADF;

	return close(fd);
}

int meter_test(char *device, char *sub_action, option_flag_t flags)
{
	int fd;

	if(device == NULL)
		return -1;

	printf("Device Name: %s\r\n", device);

	fd = meter_open(device);
	if (fd < 0)
	{
		printf("Error opening meter\r\n");
		return fd;
	}
	meter_close(fd);

	return 0;
}

int meter_read(char *device, char *sub_action, option_flag_t flags)
{

	int fd, retval = 0;
	meter_data_t data;

	if(device == NULL)
		return -1;

	fd = meter_open(device);
	if (fd < 0)
	{
		return fd;
	}
	if(sizeof(data) != read(fd, &data, sizeof(data)))
	{
		retval = -2;
	}
	else
	{
		printf("Read %d from meter\r\n", data.payload);
	}
	

	meter_close(fd);
	return 0;
}

/*
	Print the info from the device driver.
*/
int meter_info(char *device, char *sub_action, option_flag_t flags)
{
	

}
