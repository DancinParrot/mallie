
KERNEL ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CONFIG_MODULE_SIG=n # Disable module verification

obj-m += mallie.o

all:
	make -C $(KERNEL) M=$(PWD) modules

clean:
	make -C $(KERNEL) M=$(PWD) clean
