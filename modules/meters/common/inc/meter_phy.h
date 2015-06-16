#ifndef __METER_PHY_H__
#define __METER_PHY_H__

#include <linux/mutex.h>
#include <linux/types.h>

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
	GPIO_ALT_FUNC_5 = 2
}gpio_function_t;

#endif
