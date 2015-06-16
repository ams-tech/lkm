
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

#include <linux/mutex.h>
#include <linux/slab.h>
#include "meter.h"
#include "meter_phy.h"
#include <asm/io.h>

/****************************************

This is all pretty Broadcom specific and not really 'common', but 
I'm not planning on porting this to anything besides the rpi2, so
this can be our common phy driver.

*****************************************/

#define PINS_PER_FSEL	10

#define FSEL_START_ADDR	0x7E200060
#define GPSET_START_ADDR 0x7E20001C
#define GPCLR_START_ADDR 0x7E200028
#define GPLEV_START_ADDR 0x7E200034

#define GPIO_FUNC_BITFIELD 7
#define BITS_PER_REG 32

static DEFINE_MUTEX(gpio_lock);

meter_error_t set_gpio_function(u8 gpio_num, gpio_function_t func)
{
	void * remap;
	u32 temp;
	u8 offset;
	
	if(gpio_num >= NUM_GPIO)
		return METER_OUT_OF_RANGE;

	remap = ioremap_nocache(FSEL_START_ADDR + (4 * (gpio_num / PINS_PER_FSEL)), sizeof(u32));
	if(remap == NULL)
		return METER_UNKNOWN_ERROR;

	offset = 3 * (gpio_num % PINS_PER_FSEL);
	mutex_lock(&gpio_lock);

	temp = ioread32(remap);
	temp &= ~(GPIO_FUNC_BITFIELD << offset);
	temp |= (((u32)func) << offset);
	iowrite32(temp, remap);

	mutex_unlock(&gpio_lock);
	iounmap(remap);

	return METER_SUCCESS;
}

meter_error_t get_gpio_pin(u32 gpio_num, bool * value)
{
	void * remap;
        u8 offset;

        if(gpio_num >= NUM_GPIO)
                return METER_OUT_OF_RANGE;

        remap = ioremap_nocache(GPLEV_START_ADDR + (4 * (gpio_num / BITS_PER_REG)), sizeof(u32));
        if (remap == NULL)
                return METER_UNKNOWN_ERROR;

	offset = gpio_num % BITS_PER_REG;

	*value = (ioread32(remap) & (1 << offset)) != 0;
	iounmap(remap);

        return METER_SUCCESS;

}

meter_error_t set_gpio_pin(u32 gpio_num)
{
	void * remap;
	u8 offset;

	if(gpio_num >= NUM_GPIO)
		return METER_OUT_OF_RANGE;

	remap = ioremap_nocache(GPSET_START_ADDR + (4 * (gpio_num / BITS_PER_REG)), sizeof(u32));
	if (remap == NULL)
		return METER_UNKNOWN_ERROR;

	offset = gpio_num % BITS_PER_REG;
	iowrite32((1 << offset), remap);
	iounmap(remap);

	return METER_SUCCESS;
}

meter_error_t clear_gpio_pin(u32 gpio_num)
{
	void * remap;
        u8 offset;

        if(gpio_num >= NUM_GPIO)
                return METER_OUT_OF_RANGE;

        remap = ioremap_nocache(GPCLR_START_ADDR + (4 * (gpio_num / BITS_PER_REG)), sizeof(u32));
        if (remap == NULL)
                return METER_UNKNOWN_ERROR;

        offset = gpio_num % BITS_PER_REG;
        iowrite32((1 << offset), remap);
        iounmap(remap);

	return METER_SUCCESS;
}


