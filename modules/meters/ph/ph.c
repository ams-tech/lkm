#include "meter.h"
#include "meter_fops.h"
#include "meter_phy.h"
#include <linux/mutex.h>
#include "mcp3301.h"

static meter_fops_t my_fops =
{
	.init = &fops_init
};

static mcp3301_t chip =
{
	.gpio.clk = 10,
	.gpio.data = 9,
	.gpio.enable = 11
};

static meter_interface_t my_meter = 
{
	.data.mcp3301 = &chip,
	.init = &mcp3301_startup,
	.read = &mcp3301_read,
	.exit = &mcp3301_exit
};

meter_dev_t ph_meter_dev =
{
	.meter = &my_meter,
	.init = &generic_dev_init,
	.fops = &my_fops,
};
