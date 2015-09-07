#include "mcp3301.h"
#include <linux/types.h>
#include "meter_phy.h"
#include <linux/delay.h>
#include "meter.h"

#define CS_DISABLE_TIME_NS	580
#define CS_ENABLE_HOLD_TIME_NS	100
#define CLOCK_HALF_PERIOD_NS	300

static inline bool read_bit(mcp3301_t * chip)
{
	bool retval;

	set_gpio_pin(chip->gpio.clk);
        ndelay(CLOCK_HALF_PERIOD_NS);
	get_gpio_pin(chip->gpio.data, &retval);
        clear_gpio_pin(chip->gpio.clk);
        ndelay(CLOCK_HALF_PERIOD_NS);

	return retval;
}

static u16 read_voltage(mcp3301_t * chip)
{
	u16 retval = 0;
	int x;

	mutex_lock(&(chip->lock));

	set_gpio_function(chip->gpio.data, GPIO_INPUT);

	clear_gpio_pin(chip->gpio.clk);
	set_gpio_function(chip->gpio.clk, GPIO_OUTPUT);

	set_gpio_pin(chip->gpio.enable);
	set_gpio_function(chip->gpio.enable, GPIO_OUTPUT);

	ndelay(CS_DISABLE_TIME_NS);

	clear_gpio_pin(chip->gpio.enable);

	ndelay(CS_ENABLE_HOLD_TIME_NS);

	/* fCLK = 800kHz.  Half-period is 625 ns */

	/* Two blank bits */
	read_bit(chip);
	read_bit(chip);

	/* a null byte, but always 0*/
	if(read_bit(chip))
	{
		retval = MCP3301_ERROR;
		goto exit;
	}

	for(x = 0; x < RESULT_BITS; x++)
	{
		if(read_bit(chip))
			retval |= 1 << (RESULT_BITS - 1 - x);
	}

exit:
	mutex_unlock(&(chip->lock));
	return retval;
}

meter_error_t mcp3301_startup(chip_data_t data)
{
	mutex_init(&(data.mcp3301->lock));
	return METER_SUCCESS;
}

void mcp3301_exit(chip_data_t data)
{
	
}

meter_error_t mcp3301_read(chip_data_t data, meter_data_t * result)
{
	result->sig_bits = RESULT_BITS;
	result->payload = read_voltage(data.mcp3301);
	result->is_signed = true;
	result->is_big_endian = true;
	if(result->payload == MCP3301_ERROR)
		return METER_UNKNOWN_ERROR;
	else
		return METER_SUCCESS;
}
