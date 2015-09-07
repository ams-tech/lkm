#include "mcp3301.h"
#include <linux/types.h>
#include "meter_phy.h"
#include <linux/delay.h>
#include "meter.h"
#include <linux/kernel.h>

#define METER_TIMING_FUDGE	1

#define CS_DISABLE_TIME_NS	(580 * METER_TIMING_FUDGE)
#define CS_ENABLE_HOLD_TIME_NS	(100 * METER_TIMING_FUDGE)
#define CLOCK_HALF_PERIOD_NS	(300)

static inline bool read_bit(mcp3301_t * chip)
{
	bool retval;

	get_gpio_pin(chip->gpio.data, &retval);
	set_gpio_pin(chip->gpio.clk);
        mdelay(CLOCK_HALF_PERIOD_NS);
	get_gpio_pin(chip->gpio.data, &retval);
        clear_gpio_pin(chip->gpio.clk);
        mdelay(CLOCK_HALF_PERIOD_NS);
	

	return retval;
}

static u16 read_voltage(mcp3301_t * chip)
{
	u16 retval = 0;
	int x;

	mutex_lock(&(chip->lock));

	printk(KERN_DEBUG "Initializing meter GPIO\n");

	set_gpio_function(chip->gpio.data, GPIO_INPUT);

	clear_gpio_pin(chip->gpio.clk);
	set_gpio_function(chip->gpio.clk, GPIO_OUTPUT);

	set_gpio_pin(chip->gpio.enable);
	set_gpio_function(chip->gpio.enable, GPIO_OUTPUT);

	printk(KERN_DEBUG "Init complete, lowering chip enable\n");

	udelay(CS_DISABLE_TIME_NS);

	clear_gpio_pin(chip->gpio.enable);

	udelay(CS_ENABLE_HOLD_TIME_NS);

	printk(KERN_DEBUG "Reading two blank bits\n");

	/* Two blank bits */
	read_bit(chip);
	read_bit(chip);

	printk(KERN_DEBUG "Reading NULL byte\n");

	/* a null byte, but always 0*/
	if(read_bit(chip))
	{
		retval = MCP3301_ERROR;
		goto exit;
	}

	for(x = 0; x < RESULT_BITS; x++)
	{
		printk(KERN_DEBUG "Reading bit number %d\n", x);
		if(read_bit(chip))
			retval |= 1 << (RESULT_BITS - 1 - x);
	}

	printk(KERN_DEBUG "Read complete, setting chip select\n");

	set_gpio_pin(chip->gpio.enable);

	printk(KERN_DEBUG "Freeing the mutex and exiting MCP3301 read\n");

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
