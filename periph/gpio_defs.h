#ifndef __GPIO_DEFS_H
#define __GPIO_DEFS_H

#include "../platform/comip_gpio.h"

#define CMT_CSB_GPIO_PIN            170
#define CMT_FCSB_GPIO_PIN           224
#define CMT_SCLK_GPIO_PIN           171
#define CMT_SDIO_GPIO_PIN           172
#define CMT_GPIO1_GPIO_PIN          225
#define CMT_GPIO2_GPIO_PIN          225
#define CMT_GPIO3_GPIO_PIN          226
#define CMT_GPIO4_GPIO_PIN          226

#define LED1_GPIO_PIN               219
#define LED2_GPIO_PIN               220
#define LED3_GPIO_PIN               221



#define SET_GPIO_OUT(x)             comip_gpio_direction_output(x, 1)
#define SET_GPIO_IN(x)              comip_gpio_direction_input(x)
#define SET_GPIO_OD(x)              comip_gpio_direction_output(x, 1)
#define SET_GPIO_AIN(x)             comip_gpio_direction_input(x)
#define SET_GPIO_AFOUT(x)           comip_gpio_direction_output(x, 1)
#define SET_GPIO_AFOD(x)            comip_gpio_direction_output(x, 1)
#define SET_GPIO_H(x)               comip_gpio_set(x, 1)
#define SET_GPIO_L(x)               comip_gpio_set(x, 0)
#define READ_GPIO_PIN(x)            comip_gpio_get(x)

#endif
