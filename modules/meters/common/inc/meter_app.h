#ifndef __METER_APP_H__
#define __METER_APP_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
typedef uint8_t u8;
typedef uint32_t u32;
#endif

#define MODULE_NAME	"meters"

typedef struct meter_data_X
{
	u8 sig_bits;
	u32 payload;
}meter_data_t;

#endif
