#ifndef __METER_FOPS_H__
#define __METER_FOPS_H__

#include "meter.h"
#include <linux/fs.h>
#include <linux/module.h>

typedef struct meter_fops_X
{
	struct file_operations fops;
	meter_init_f init;
}meter_fops_t;
/*
const struct file_operations default_fops = 
{
	.owner = THIS_MODULE,
	.llseek = default_llseek,
	.read = default_read,
	.write = default_write,
	.ioctl = default_ioctl,
	.open = default_open,
	.release = default_release,
	.aio_read = default_aio_read,
	.aio_write = default_aio_write
}
*/
meter_error_t generic_fops_init(meter_dev_t * dev);

#endif
