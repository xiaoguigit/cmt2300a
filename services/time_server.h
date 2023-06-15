#ifndef __TIME_SERVER_H
#define __TIME_SERVER_H
#include <stdint.h>
#include "typedefs.h"

inline uint32_t getTick(void);
void system_delay_ms(u32 n);
void system_delay_10us(u32 n);
void system_delay_100us(u32 n);
void system_delay_us(u32 n);
void system_delay_500ns(void);


#endif
