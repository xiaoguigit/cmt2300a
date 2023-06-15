#include <string.h>
#include "time_server.h"
#include "gpio_defs.h"



inline uint32_t getTick(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void system_delay_ms(u32 nms)
{
    usleep(nms * 1000);
}

void system_delay_10us(u32 n)
{
    usleep(n * 10);
}

void system_delay_100us(u32 n)
{
    usleep(n * 100);
}

void system_delay_us(u32 n)
{
    usleep(n);
}

void system_delay_500ns(void)
{
    usleep(500);
}
