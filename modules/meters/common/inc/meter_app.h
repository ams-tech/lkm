#ifndef __METER_APP_H__
#define __METER_APP_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint32_t u32;
#endif

#define MODULE_NAME	"meters"

typedef struct meter_data_X
{
	u8 sig_bits;
	u32 payload;
	bool is_signed;
	bool is_big_endian;
}meter_data_t;

#define METER_MAGIC_NUMBER	'g'

/*
 * _IOR means that we're creating an ioctl command 
 * number for passing information from a user process
 * to the kernel module. 
 *
 * _IOW means we're passing kernel information to the user
 *
 * Commands that start with a B_ are buffered commands.  
 * Access them via get/set_buffered_result
 */

/*
 * Get device info for the user.
 *
 */
#define B_IOCTL_GET_METER_INFO _IOW(METER_MAGIC_NUMBER, 1, int)


#define IOCTL_GET_BUF 		_IOR(METER_MAGIC_NUMBER, 4, void *)
#define IOCTL_BUF_RESET		_IO(METER_MAGIC_NUMBER, 5)

#define IOCTL_GET_MAGIC _IOW(METER_MAGIC_NUMBER, 6, char)

#ifndef __KERNEL__

/* These functions are used to safely get/return data to/from the driver */

/* 
	input: ioctl_num - one of the IOCTL defines listed above
	output: *buf - a pointer to a buffer allocated in app space

	return: The length of a buffer, or an error code if negative
*/
static inline int get_buffered_result(int fd, unsigned int ioctl_num, void *buf)
{
	int retval, len;

	retval = ioctl(fd, ioctl_num, &len);
	if(retval < 0)
		return retval;

	buf = malloc(len);

	if(buf == NULL)
	{
		if(ioctl(fd, IOCTL_BUF_RESET) < 0)
			printf("ERROR: Unable to reset the buffer.\r\n\t\
				The device may have locked up or reset!");
		return -ENOMEM;
	}

	retval = ioctl(fd, IOCTL_GET_BUF, buf);

	if(retval < 0)
	{
		free(buf);
		buf = NULL;
		return retval;
	}
	return len;
}

#endif

#endif
