# Makefile toolchain part
WC=$(wildcard *.c)
TOOLCHAIN_PATH=/buildroot/output/host/usr/bin/
TOOLCHAIN=$(TOOLCHAIN_PATH)aarch64-linux-
APPNAME=app
# Makefile common part
CC=$(TOOLCHAIN)gcc
LD=$(TOOLCHAIN)gcc
AR=$(TOOLCHAIN)ar
CFLAGS+=-Wall -Wextra -g -mcpu=cortex-a53 -O0 -MD -std=gnu11
OUTPUT=-o $(APPNAME)
all:
	$(CC)	$(CFLAGS)	$(OUTPUT)	$(wildcard *.c)