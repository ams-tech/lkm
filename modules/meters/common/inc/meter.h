
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


#ifndef __METER_H__
#define __METER_H__

#include <linux/types.h>
#include <linux/cdev.h>


typedef enum meter_error_X
{
	METER_SUCCESS = 0,
	METER_UNKNOWN_ERROR,
}meter_error_t;

/* Forward-reference structures go here */
struct meter_fops_X;
struct meter_dev_X;

/*A generic init function pointer type*/
typedef meter_error_t (*meter_init_f)(struct meter_dev_X * dev);

typedef struct meter_dev_X
{
	struct meter_fops_X * fops;
	meter_init_f const init;
	dev_t dev_num;
}meter_dev_t;


#define METER_MAJOR_ID	0

#ifndef METER_NUM_DEVS
	#error "Error: METER_NUM_DEVS must be defined at compile time!"
#endif

#ifndef METER_DEV_INIT
	#error "ERROR: METER_DEV_INIT must be defined at compile time!"
#endif

#ifndef METER_DEV_EXTERNS
	#error "Error: METER_DEV_EXTERNS must be defined at compile time!"
#else
METER_DEV_EXTERNS
#endif

#define MODULE_NAME	"meters"

meter_error_t generic_dev_init(meter_dev_t * dev);

extern int major_id;
extern meter_dev_t * devices_in_ram[];

#endif
