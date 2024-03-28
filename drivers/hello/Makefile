KEV = $(shell uname -r)

obj-m := hello.o

build: modules

modules:
	$(MAKE) -C /lib/modules/$(KEV)/build M=$(shell pwd) modules

clean:
	$(MAKE) -C /lib/modules/$(KEV)/build M=$(shell pwd) clean
