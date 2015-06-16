#ifndef __METER_PHY_H__
#define __METER_PHY_H__

#include <linux/mutex.h>
#include <linux/types.h>

#include "meter.h"

#define NUM_GPIO	41

typedef enum gpio_function_X
{
	GPIO_INPUT = 0,
	GPIO_OUTPUT = 1,
	GPIO_ALT_FUNC_0 = 4,
	GPIO_ALT_FUNC_1 = 5,
	GPIO_ALT_FUNC_2 = 6,
	GPIO_ALT_FUNC_3 = 7,
	GPIO_ALT_FUNC_4 = 3,
	GPIO_ALT_FUNC_5 = 2,
}gpio_function_t;

meter_error_t clear_gpio_pin(u32 gpio_num);
meter_error_t set_gpio_pin(u32 gpio_num);
meter_error_t get_gpio_pin(u32 gpio_num, bool * value);
meter_error_t set_gpio_function(u32 gpio_num, gpio_function_t func);

#endif
