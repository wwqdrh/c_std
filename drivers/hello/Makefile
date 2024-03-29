KEV = $(shell uname -r)
PWD = $(shell pwd)

obj-m := hello.o

hello-objs := main.o ../../src/container/linklist.o

ccflags-y += -I$(obj)/../../include/
ccflags-y += -DKERNELMODE

build: modules

modules:
	$(MAKE) -C /lib/modules/$(KEV)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(KEV)/build M=$(PWD) clean
