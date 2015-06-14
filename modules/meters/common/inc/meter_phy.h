#ifndef __METER_PHY_H__
#define __METER_PHY_H__

#include <linux/mutex.h>
#include <linux/types.h>

typedef struct phy_reg_X
{
	u32 address;
	struct mutex lock;
	void * remap;
}phy_reg_t;

typedef struct phy_reg_list_X
{
	phy_reg_t * reg;
	struct phy_reg_list_X * next;
}phy_reg_list_t;

typedef struct gpio_pin_X
{
	phy_reg_t * data_direction;
	phy_reg_t * data;
	phy_reg_t * open_drain;
	u8 bit;
}gpio_pin_t;

typedef struct gpio_pin_list_X
{
	gpio_pin_t * pin;
	struct gpio_pin_list_X * next;
}gpio_pin_list_t;

typedef struct dev_phy_X
{
	struct mutex lock;
	phy_reg_list_t * owned_reg_list;
	gpio_pin_list_t * pin_list;
}dev_phy_t;

#endif
