
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

static DEFINE_MUTEX(phy_lock);
static phy_reg_list_t * reg_head = NULL;

static meter_error_t checkout_phy_reg(dev_phy_t * dev_phy, phy_reg_t * reg)
{
	/*Note: Only call this fucnction when you own the phy and dev_phy locks*/
	phy_reg_list_t ** temp = &(dev_phy->owned_reg_list);

	while(*temp != NULL)
	{
		if((*temp)->reg == reg)
			return METER_SUCCESS;
		temp = &((*temp)->next);
	}

	/* We don't own this register yet.  Get the lock */
	mutex_lock(&(reg->lock));
	*temp = kmalloc(sizeof(phy_reg_list_t), GFP_KERNEL);

	if (*temp == NULL)
	{
		mutex_unlock(&(reg->lock));
		return METER_ALLOC_ERROR;
	}

	(*temp)->next = NULL;
	(*temp)->reg = reg;

	reg->remap = ioremap_nocache((unsigned long)reg->address, sizeof(u32));
	return METER_SUCCESS;
}

void release_dev_phy(dev_phy_t * dev_phy)
{
	phy_reg_list_t ** temp = &(dev_phy->owned_reg_list);

	if(!mutex_is_locked(&(dev_phy->lock)))
		return;

	while(*temp != NULL)
	{
		phy_reg_list_t ** cur = temp;
		phy_reg_t * cur_reg = (*temp)->reg;
		
		temp = &((*temp)->next);

		kfree(*cur);
		*cur = NULL;

		iounmap(cur_reg->remap);
		cur_reg->remap = NULL;
		mutex_unlock(&(cur_reg->lock));
	}


	mutex_unlock(&(dev_phy->lock));
	return;
}

meter_error_t checkout_dev_phy(dev_phy_t * dev_phy)
{
	meter_error_t retval = METER_SUCCESS;
	gpio_pin_list_t ** temp = &(dev_phy->pin_list);
	
	mutex_lock(&(dev_phy->lock));
	
	mutex_lock(&phy_lock);

	while(*temp != NULL)
	{
		gpio_pin_t * cur = (*temp)->pin;

		retval = checkout_phy_reg(dev_phy, cur->data_direction);
		if(retval != METER_SUCCESS)
			goto exit;

		retval = checkout_phy_reg(dev_phy, cur->data);
                if(retval != METER_SUCCESS)
                        goto exit;

		retval = checkout_phy_reg(dev_phy, cur->open_drain);
                if(retval != METER_SUCCESS)
                        goto exit;


		temp = &((*temp)->next);
	}

exit:
	mutex_unlock(&phy_lock);
	return retval;
}

phy_reg_t * get_phy_reg(u32 address)
{
	phy_reg_t * retval = NULL;
	phy_reg_list_t ** temp = &reg_head;

	mutex_lock(&phy_lock);

	while(*temp != NULL)
	{
		if((*temp)->reg->address == address)
		{
			retval = (*temp)->reg;
			goto exit;
		}
		temp = &((*temp)->next);
	}	

	*temp = kmalloc(sizeof(phy_reg_list_t), GFP_KERNEL);
	if (*temp == NULL)
		goto exit;
	(*temp)->next = NULL;

	/*Register not found -- create and initialize a new one*/
	retval = kmalloc(sizeof(phy_reg_t), GFP_KERNEL);
	if(retval != NULL)
	{
		retval->address = address;
		mutex_init(&(retval->lock));
		(*temp)->reg = retval;
		retval->remap = NULL;
	}
	else
	{
		kfree(*temp);
		*temp = NULL;
	}

exit:
	mutex_unlock(&phy_lock);
	return retval;
}
