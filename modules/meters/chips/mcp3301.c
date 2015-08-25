#include "mcp3301.h"
#include <linux/types.h>
#include "meter_phy.h"
#include <linux/delay.h>
#include "meter.h"

#define HALF_PERIOD_NS	625
#define TSCH_NS	625
#define TSUCS_NS	100


static inline bool read_bit(mcp3301_t * chip)
{
	bool retval;

	set_gpio_pin(chip->gpio.clk);
        ndelay(HALF_PERIOD_NS);
	get_gpio_pin(chip->gpio.data, &retval);
        clear_gpio_pin(chip->gpio.clk);
        ndelay(HALF_PERIOD_NS);

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

	/* Wait for tCSH - 625 ns */
	ndelay(TSCH_NS);

	clear_gpio_pin(chip->gpio.enable);

	/* wait tSUCS - 100 ns */
	ndelay(TSUCS_NS);

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
	if(result->payload == MCP3301_ERROR)
		return METER_UNKNOWN_ERROR;
	else
		return METER_SUCCESS;
}
