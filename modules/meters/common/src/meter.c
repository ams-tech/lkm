/***********************************

    meter - a driver for a simple meter device
    Copyright (C) <2015>  <ams-tech>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

***********************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>
#include "meter.h"

int __init meter_init(void);
void __exit meter_exit(void);

int major_id = METER_MAJOR_ID;
int num_devs = METER_NUM_DEVS;

module_param(major_id, int, 0);

MODULE_AUTHOR("ams-tech");
MODULE_LICENSE("GPL v2");

module_init(meter_init);
modul_exit(meter_exit);

meter_dev_t devices_in_ram[METER_NUM_DEVS] = METER_DEV_INIT;

int __init meter_init(void)
{
	int result;
	dev_t dev_id = MKDEV(major_id, 0);

	/* Register the major ID */
	if(major_id != 0)
		result = register_chrdev_region(dev_id, num_devs, MODULE_NAME);
	else
	{
		result = alloc_chrdev_region(&dev, 0, num_devs, MODULE_NAME);
		major_id = MAJOR(dev);
	}
	if (result < 0)
		return result;
	
	

}







