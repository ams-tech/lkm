#ifndef __METER_PHY_H__
#define __METER_PHY_H__

#include <linux/mutex.h>
#include <linux/types.h>

typedef struct phy_reg_X
{
	u32 address;
	struct mutex lock;
}phy_reg_t;

typedef struct phy_reg_list_X
{
	phy_reg_t * reg;
	struct phy_reg_list_X * next;
}phy_reg_list_t;

phy_reg_t * get_phy_reg(u32 address);

#endif
