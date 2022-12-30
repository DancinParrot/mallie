
KERNEL ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CONFIG_MODULE_SIG=n # Disable module verification

obj-m += mallie-lkm.o

# c99 let's compiler know we're using the standard C released in 1999
# -Wall enables all warnings
# pedantic will issue all the warnings demanded by strict ISO C and ISO C++
# pendantic-errors will turn all warnings into errors

all:
	mallie-lkm mallie
mallie-lkm: 
	make -C $(KERNEL) M=$(PWD) modules
mallie:
	gcc -o mallie mallie.c -std=c99 -Wall -pedantic -Wextra
clean:
	make -C $(KERNEL) M=$(PWD) clean
