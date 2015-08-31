#ifndef __DEVICE_ACTIONS_H__
#define __DEVICE_ACTIONS_H__

typedef enum device_error_X
{
	DEVICE_SUCCESS = 0,
	DEVICE_INVALID_PARAM,
	DEVICE_READ_ERROR,
}device_error_t;

int device_open(char *device_name);
int device_close(int fd);
device_error_t read_value(int fd, float *value);

#endif
