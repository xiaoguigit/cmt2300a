#include "comip_gpio.h"


static unsigned int *gpioBase = NULL;
static unsigned int *pinctrlBase = NULL;

static unsigned int *mapGpioBase(void)
{
    unsigned int fd;
    unsigned int addrStart, addrOffset;
    unsigned int pageMask;

    if((fd = open("/dev/mem",O_RDWR)) == -1){
        return NULL;
    }

    unsigned int pageSize = sysconf(_SC_PAGESIZE);
    pageMask = (~(pageSize - 1));
    addrStart = GPIO_BASE_ADDRESS & pageMask;
    addrOffset = GPIO_BASE_ADDRESS & ~pageMask;

    if((gpioBase = mmap(NULL, pageSize*2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addrStart)) == NULL){
        close(fd);
        return NULL;
    }

    close(fd);
    return gpioBase;
}


static void munmapGpioBase(void)
{
    unsigned int size;
    size = sysconf(_SC_PAGESIZE);
    munmap(gpioBase, size);
    gpioBase = NULL;
}



static unsigned int *mapPinctrlBase(void)
{
    unsigned int fd;
    unsigned int addrStart, addrOffset;
    unsigned int pageMask;

    if((fd = open("/dev/mem",O_RDWR)) == -1){
         return NULL;
    }

    unsigned int pageSize = sysconf(_SC_PAGESIZE);
    pageMask = (~(pageSize - 1));
    addrStart = PINCTRL_BASE_ADDRESS & pageMask;
    addrOffset = PINCTRL_BASE_ADDRESS & ~pageMask;

    if((pinctrlBase = mmap(NULL, pageSize*2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addrStart)) == NULL){
        close(fd);
        return NULL;
    }

    close(fd);
    return pinctrlBase;
}


static void munmapPinctrlBase(void)
{
    unsigned int size;
    size = sysconf(_SC_PAGESIZE);
    munmap(pinctrlBase, size);
    pinctrlBase = NULL;
}




int comip_gpio_get(unsigned offset)
{
    unsigned int *reg;
    unsigned int bit;
    unsigned int val;

    if(gpioBase == NULL){
        gpioBase = mapGpioBase();
    }

    reg = (unsigned int *)((char *)gpioBase + GPIO_EXT_PORT(offset));
    bit = offset % 32;
    val = *reg & (1 << bit);

    return !!val;
}

void comip_gpio_set(unsigned offset, int value)
{
    unsigned int *reg;
    unsigned int bit;
    unsigned int val;

    if(gpioBase == NULL){
        gpioBase = mapGpioBase();
    }

    reg = (unsigned int *)((char *)gpioBase + GPIO_PORT_DR(offset));
    bit = offset % 16;
    val = (1 << (bit + 16));
    if(value){
        val |= 1 << bit;
    }
    *reg = val;
}


void comip_muxpin_set_mux(unsigned pin, unsigned int mux)
{
    unsigned int *reg;
    unsigned int val = 0;

    if(pinctrlBase == NULL){
        pinctrlBase = mapPinctrlBase();
    }

    if(pin < 256){
        reg = (unsigned int *)((char *)pinctrlBase + ((pin) * 4));
    }else{
        reg = (unsigned int *)((char *)pinctrlBase + 0x480 + (((pin)-256) * 4));
    }
    val = *reg;
    val = (val & (~MUXPIN_MUX_MASK)) | MUXPIN_MUX(mux);
    *reg = val;

    return;
}


int comip_gpio_direction_input(unsigned offset)
{
    unsigned int *reg;
    unsigned int bit;
    unsigned int val;

    comip_muxpin_set_mux(offset, 2);

    if(gpioBase == NULL){
        gpioBase = mapGpioBase();
    }
    reg = (unsigned int *)((char *)gpioBase + GPIO_PORT_DDR(offset));
    bit = offset % 16;
    val = (1 << (bit + 16));
    *reg = val;

    return 0;
}

int comip_gpio_direction_output(unsigned offset, int value)
{
    unsigned int *reg;
    unsigned int bit;
    unsigned int val;

    comip_muxpin_set_mux(offset, 2);

    /* Set output value. */
    comip_gpio_set(offset, value);

    if(gpioBase == NULL){
        gpioBase = mapGpioBase();
    }
    /* Set direction. */
    reg = (unsigned int *)((char *)gpioBase + GPIO_PORT_DDR(offset));
    bit = offset % 16;
    val = (1 << (bit + 16)) | (1 << bit);
    *reg = val;

    return 0;
}


