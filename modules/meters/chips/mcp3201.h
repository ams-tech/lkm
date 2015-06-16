#ifndef __MCP3201_H__
#define __MCP3201_H__

#include <linux/types.h>
#include <linux/mutex.h>

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

#endif
