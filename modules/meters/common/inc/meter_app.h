#ifndef __METER_APP_H__
#define __METER_APP_H__

#include <linux/types.h>

typedef struct meter_data_X
{
	u8 sig_bits;
	u32 payload;
}meter_data_t;

#endif
