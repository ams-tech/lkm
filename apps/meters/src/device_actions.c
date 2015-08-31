#include "meter_app.h"
#include "module/meter_app.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include "device_actions.h"

int device_open(char *device)
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

int device_close(int fd)
{
        if(fd < 0)
                return -EBADF;

        return close(fd);
}

device_error_t read_value(int fd, float *value)
{
	meter_data_t data;
	long result = 0;

	if(fd <= 0)
	{
		return DEVICE_INVALID_PARAM;
	}
	
	/* the device should read out *exactly* the size of meter_data_t */
	if(sizeof(data) != read(fd, &data, sizeof(data)))
	{
		return DEVICE_READ_ERROR;
	}
	else
	{
                int i;
                bool negative = false;
                for (i = 0; i < data.sig_bits; i++)
                {
                        int device_bit;
                        if(data.is_big_endian)
                        {
                                device_bit = (data.sig_bits - i);
                        }
                        else
                        {
                                device_bit = i;
                        }

                        if((device_bit == (data.sig_bits - 1)) && (data.is_signed))
                        {
                                negative = (data.payload & (1 << device_bit)) != 0;
                        }
                        else
                        {
                                result |= (data.payload & (1 << device_bit));
                        }
                }

                if(negative)
                {
                        //Two's complement magic
                        result = ((1 << (data.sig_bits - 1)) * -1) + result;
                }
	}

	printf("Device has %d sig bits\r\n", data.sig_bits);

	if(data.is_signed)
	{
		*value = ((float)result) / ((float)(1 << (data.sig_bits - 1)));
	}
	else
	{
		*value = ((float)result) / ((float)(1 << (data.sig_bits)));
	}

	return DEVICE_SUCCESS;
}









