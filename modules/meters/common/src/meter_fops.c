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

#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include "meter_fops.h"
#include "meter_phy.h"
#include "meter_app.h"

int default_open(struct inode *, struct file *);
int default_release(struct inode *, struct file *);
ssize_t default_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
long default_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

const struct file_operations default_fops =
{
	.owner = THIS_MODULE,
	.read = default_read,
	.write = NULL,
	.unlocked_ioctl = default_ioctl,
	.open = default_open,
	.release = default_release
};

/* This function sets up our cdev.  Any replacement function should do the same */
meter_error_t fops_init(meter_dev_t * dev)
{
	int result;

	cdev_init(&(dev->fops->cdev), &default_fops);
	
	result = cdev_add(&(dev->fops->cdev), dev->dev_num, 1);

	if (result)
	{
		printk(KERN_NOTICE "Error adding a meter device");
		return METER_UNKNOWN_ERROR;
	}
	return METER_SUCCESS;
}

void fops_exit(meter_dev_t * dev)
{
	cdev_del(&(dev->fops->cdev));
}

long default_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

ssize_t default_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	meter_dev_t * dev = filp->private_data;
	meter_data_t data;

	if(dev == NULL)
		return -ENODEV;

	if(METER_SUCCESS != dev->meter->read(dev->meter->data, &data))
		return -ENODEV;

	if(copy_to_user(buf, &data, sizeof(meter_data_t)))
		return -EFAULT;

	return sizeof(meter_data_t);
}

int default_release(struct inode *inode, struct file *filp)
{
	return 0;
}

int default_open(struct inode *inode, struct file *filp)
{
	filp->private_data = NULL;

	if(major_id == imajor(inode))
	{
		unsigned int minor = iminor(inode);
		if(minor < METER_NUM_DEVS)
			filp->private_data = devices_in_ram[minor];
	}

	if(filp->private_data == NULL)
		return -ENODEV;

	return 0;
}




