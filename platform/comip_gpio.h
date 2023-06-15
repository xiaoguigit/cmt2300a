#ifndef __GPIO_H__
#define __GPIO_H__
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define GPIO_BASE_ADDRESS           0xE450C000
#define PINCTRL_BASE_ADDRESS        0xE4509000

#define GPIO_REG(base, gpio, num)   (base + ((gpio) / num) * 4)

/* Registers. */
#define GPIO_PORT_DR(gpio)          GPIO_REG(0x00,  gpio, 16)
#define GPIO_PORT_DDR(gpio)         GPIO_REG(0x48,  gpio, 16)
#define GPIO_EXT_PORT(gpio)         GPIO_REG(0x90,  gpio, 32)
#define GPIO_DEBOUNCE(gpio)         GPIO_REG(0x1D4, gpio, 16)
#define GPIO_INTR_CTRL(gpio)        GPIO_REG(0xB4,  gpio, 4)
#define GPIO_INTR_RAW(gpio)         GPIO_REG(0x21C, gpio, 32)
#define GPIO_INTR_CLR(gpio)         GPIO_REG(0x240, gpio, 32)
#define GPIO_INTR_MASK(gpio)        GPIO_REG(0x3A8, gpio, 16)
#define GPIO_INTR_STATUS(gpio)      GPIO_REG(0x3F0, gpio, 32)


#define MUXPIN_REG(base, x)         (((x) < 256) ? (base + ((x) * 4)) : (base + 0x480 + (((x)-256) * 4)))
#define MUXPIN_MUX_MASK             (0x3 << 0)
#define MUXPIN_MUX(x)               (((x) << 0) & MUXPIN_MUX_MASK)

#define MUXPIN_PULL_DOWN_MASK       (0x1 << 2)
#define MUXPIN_PULL_UP_MASK         (0x1 << 3)
#define MUXPIN_PULL_MASK            (0x3 << 2)
#define MUXPIN_PULL(x)              (((x) << 2) & MUXPIN_PULL_MASK)
#define MUXPIN_PULL_OLD(x)          ((1 << (x)) & MUXPIN_PULL_MASK)
#define MUXPIN_STRONG_PULL_UP       (0x1 << 9)

#define MUXPIN_DRIVE_MASK           (0xf << 4)
#define MUXPIN_DRIVE(x)             (((x) << 4) & MUXPIN_DRIVE_MASK)


int gpioInit(void);

int comip_gpio_get(unsigned offset);

void comip_gpio_set(unsigned offset, int value);

int comip_gpio_direction_input(unsigned offset);

int comip_gpio_direction_output(unsigned offset, int value);

void comip_muxpin_set_mux(unsigned pin, unsigned int mux);

#endif