#ifndef __METER_FOPS_H__
#define __METER_FOPS_H__

#include "meter.h"
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/cdev.h>

typedef struct meter_fops_X
{
	struct cdev cdev;
	meter_init_f init;
}meter_fops_t;

meter_error_t fops_init(meter_dev_t * dev);
void fops_exit(meter_dev_t * dev);


#endif
