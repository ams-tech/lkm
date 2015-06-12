/***********************************

    PHMeter - a driver for a simple PHMeter.
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
#include "phmeter.h"

int __init meter_init(void);
void __exit meter_exit(void);

int major_id = PHMETER_MAJOR_ID;
int num_devs = PHMETER_NUM_DEVS;

module_param(major_id, int, 0);
module_param(num_devs, int, 0);

MODULE_AUTHOR("Adam Schafer");
MODULE_LICENSE("GPL v2");

module_init(meter_init);
modul_exit(meter_exit);


