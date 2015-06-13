
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

#include <linux/slab.h>
#include "meter_phy.h"

static phy_reg_list_t * reg_head = NULL;

phy_reg_t * get_phy_reg(u32 address)
{
	phy_reg_t * retval;
	phy_reg_list_t ** temp = &reg_head;


	while(*temp != NULL)
	{
		if((*temp)->reg->address == address)
		{
			return (*temp)->reg;
		}
		temp = &((*temp)->next);
	}	

	*temp = kmalloc(sizeof(phy_reg_list_t), GFP_KERNEL);
	if (*temp == NULL)
		return NULL;
	(*temp)->next = NULL;

	/*Register not found -- create and initialize a new one*/
	retval = kmalloc(sizeof(phy_reg_t), GFP_KERNEL);
	if(retval != NULL)
	{
		retval->address = address;
		mutex_init(&(retval->lock));
		(*temp)->reg = retval;
	}
	else
	{
		kfree(*temp);
		*temp = NULL;
	}

	return retval;
}
