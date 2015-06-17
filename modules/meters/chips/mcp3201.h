#ifndef __MCP3201_H__
#define __MCP3201_H__

#include <linux/types.h>
#include <linux/mutex.h>

#include "meter.h"
#include "meter_phy.h"

#define MCP3201_ERROR 0xFFFF
#define RESULT_BITS	12

typedef struct mcp3201_X
{
	struct mutex lock;
	struct gpio
	{
		u32 clk;
		u32 data;
		u32 enable;
	} gpio;
}mcp3201_t;

meter_error_t mcp3201_startup(chip_data_t data);
meter_error_t mcp3201_read(chip_data_t data, meter_data_t * result);
void mcp3201_exit(chip_data_t data);

#endif
