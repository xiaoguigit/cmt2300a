#ifndef __COMMON_H
#define __COMMON_H

#include "typedefs.h"
#include "gpio_defs.h"


void no_optimize(const void* p_param);
void Common_Init(void);
void set_u16_to_buf(u8 buf[], u16 dat16);
u16 get_u16_from_buf(const u8 buf[]);

void set_u32_to_buf(u8 buf[], u32 dat32);
u32 get_u32_from_buf(const u8 buf[]);

void views_print_line(u8 nLine, const char* str);

#endif
