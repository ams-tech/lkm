#ifndef __MCP3301_H__
#define __MCP3301_H__

#include <linux/types.h>
#include <linux/mutex.h>

#include "meter.h"
#include "meter_phy.h"

#define MCP3301_ERROR 0xFFFF
#define RESULT_BITS	13

typedef struct mcp3301_X
{
	struct mutex lock;
	struct gpio
	{
		u32 clk;
		u32 data;
		u32 enable;
	} gpio;
}mcp3301_t;

meter_error_t mcp3301_startup(chip_data_t data);
meter_error_t mcp3301_read(chip_data_t data, meter_data_t * result);
void mcp3301_exit(chip_data_t data);

#endif
