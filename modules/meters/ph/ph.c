#include "meter.h"
#include "meter_fops.h"

static meter_fops_t my_fops =
{
	.init = &fops_init
};

meter_dev_t ph_meter_dev =
{
	.init = &generic_dev_init,
	.fops = &my_fops,
};
