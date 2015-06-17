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

typedef struct meter_data_X
{
	u8 sig_bits;
	u32 payload;
}meter_data_t;

struct mcp3201_X;

typedef union chip_data_X
{
	struct mcp3201_X * mcp3201;
}chip_data_t;

typedef meter_error_t (*read_meter_f)(chip_data_t data, meter_data_t * result);
typedef meter_error_t (*meter_startup_f)(chip_data_t data);
typedef void (*meter_exit_f)(chip_data_t data);

typedef struct meter_interface_X
{
	chip_data_t data;
	read_meter_f read;
	meter_startup_f startup;
	meter_exit_f exit;
}meter_interface_t;


meter_error_t clear_gpio_pin(u32 gpio_num);
meter_error_t set_gpio_pin(u32 gpio_num);
meter_error_t get_gpio_pin(u32 gpio_num, bool * value);
meter_error_t set_gpio_function(u32 gpio_num, gpio_function_t func);

#endif
