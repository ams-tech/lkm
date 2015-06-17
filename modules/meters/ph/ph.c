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
	.gpio.clk = 0,
	.gpio.data = 0,
	.gpio.enable = 0
};

static meter_interface_t my_meter = 
{
	.data.mcp3201 = &chip,
	.read = NULL
};

meter_dev_t ph_meter_dev =
{
	.meter = &my_meter,
	.init = &generic_dev_init,
	.fops = &my_fops,
};
