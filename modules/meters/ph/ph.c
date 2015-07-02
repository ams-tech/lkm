#include "meter.h"
#include "meter_fops.h"
#include "meter_phy.h"
#include <linux/mutex.h>
#include "mcp3201.h"

static meter_fops_t my_fops =
{
	.init = &fops_init
};

static mcp3201_t chip =
{
	.gpio.clk = 10,
	.gpio.data = 9,
	.gpio.enable = 11
};

static meter_interface_t my_meter = 
{
	.data.mcp3201 = &chip,
	.init = &mcp3201_startup,
	.read = &mcp3201_read,
	.exit = &mcp3201_exit
};

meter_dev_t ph_meter_dev =
{
	.meter = &my_meter,
	.init = &generic_dev_init,
	.fops = &my_fops,
};
