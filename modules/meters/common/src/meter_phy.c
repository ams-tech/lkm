
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

#ifdef RPI_1
	#define VIRTUAL_ADDR_HEADER	0xF2000000
#else
	#define VIRTUAL_ADDR_HEADER	0x3F000000
#endif

#define FSEL_START_ADDR	(0x200000 | VIRTUAL_ADDR_HEADER)
#define GPSET_START_ADDR (0x20001C | VIRTUAL_ADDR_HEADER)
#define GPCLR_START_ADDR (0x200028 | VIRTUAL_ADDR_HEADER)
#define GPLEV_START_ADDR (0x200034 | VIRTUAL_ADDR_HEADER)

#define GPIO_FUNC_BITFIELD 7
#define BITS_PER_REG 32

static DEFINE_MUTEX(gpio_lock);

meter_error_t set_gpio_function(u32 gpio_num, gpio_function_t func)
{
	void * remap;
	u32 temp, addr;
	u8 offset;
	
	if(gpio_num >= NUM_GPIO)
		return METER_OUT_OF_RANGE;

	addr = FSEL_START_ADDR + (4 * (gpio_num / PINS_PER_FSEL));

	remap = ioremap_nocache(addr, sizeof(u32));
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

	printk(KERN_DEBUG "Set GPFSEL register 0x%08x to 0x%08x\n", addr, temp);

	return METER_SUCCESS;
}

meter_error_t get_gpio_pin(u32 gpio_num, bool * value)
{
	void * remap;
        u8 offset;
	u32 addr, reg_value;

        if(gpio_num >= NUM_GPIO)
                return METER_OUT_OF_RANGE;

	addr = GPLEV_START_ADDR + (4 * (gpio_num / BITS_PER_REG));

        remap = ioremap_nocache(addr, sizeof(u32));
        if (remap == NULL)
                return METER_UNKNOWN_ERROR;

	offset = gpio_num % BITS_PER_REG;

	reg_value = ioread32(remap);

	iounmap(remap);
	*value = (reg_value & (1 << offset)) != 0;

	printk(KERN_DEBUG "GPLEV: 0x%08x -> 0x%08x\n", addr, reg_value);

        return METER_SUCCESS;

}

meter_error_t set_gpio_pin(u32 gpio_num)
{
	void * remap;
	u8 offset;
	u32 addr, value;

	if(gpio_num >= NUM_GPIO)
		return METER_OUT_OF_RANGE;

	addr = GPSET_START_ADDR + (4 * (gpio_num / BITS_PER_REG));

	remap = ioremap_nocache(addr, sizeof(u32));
	if (remap == NULL)
		return METER_UNKNOWN_ERROR;

	offset = gpio_num % BITS_PER_REG;
	value = 1 << offset;
	iowrite32(value, remap);
	iounmap(remap);

	printk(KERN_DEBUG "GPSET: 0x%08x <- 0x%08x\n", addr, value);

	return METER_SUCCESS;
}

meter_error_t clear_gpio_pin(u32 gpio_num)
{
	void * remap;
        u8 offset;
	u32 addr, value;

        if(gpio_num >= NUM_GPIO)
                return METER_OUT_OF_RANGE;

	addr = GPCLR_START_ADDR + (4 * (gpio_num / BITS_PER_REG));

        remap = ioremap_nocache(addr, sizeof(u32));
        if (remap == NULL)
                return METER_UNKNOWN_ERROR;

        offset = gpio_num % BITS_PER_REG;
	value = 1 << offset;
        iowrite32(value, remap);
        iounmap(remap);

	printk(KERN_DEBUG "GPCLR: 0x%08x <- 0x%08x\n", addr, value);

	return METER_SUCCESS;
}


