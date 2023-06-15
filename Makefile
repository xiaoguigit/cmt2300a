
CROSS_COMPILE = arm-linux-gnueabi-
TOPDIR        := $(shell pwd)

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CXX		= $(CROSS_COMPILE)g++
CPP		= $(CROSS_COMPILE)g++
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

TOPDIR := $(shell pwd)
export TOPDIR

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -g -rdynamic -funwind-tables -ffunction-sections -fgnu89-inline -Wno-error=deprecated-declarations -Wno-deprecated-declarations
#CFLAGS := -g -DDEBUG -rdynamic -funwind-tables -ffunction-sections
CFLAGS += -I$(TOPDIR)/periph -I$(TOPDIR)/platform -I$(TOPDIR)/radio -I$(TOPDIR)/services

LDFLAGS := -lpthread

export CFLAGS LDFLAGS

TARGET := test


obj-y += periph/
obj-y += platform/
obj-y += radio/
obj-y += services/


all :
	rm -rf $(INSTALLDIR)
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o


install:
	rm -rf $(INSTALLDIR)
	install -d $(INSTALLDIR)/bin
	install -d $(INSTALLDIR)/etc
	install -d $(INSTALLDIR)/scripts
	install -m 0755 $(TARGET) $(INSTALLDIR)/bin
	install -m 0755 oam_config.json $(INSTALLDIR)/etc
	install -m 0755 oam_callback.sh $(INSTALLDIR)/scripts

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(shell find -name ".*.o.d")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(shell find -name ".*.o.d")
	rm -f $(TARGET)
