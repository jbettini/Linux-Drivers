PWD := $(shell pwd)
NAME := key_driver
KDIR := /lib/modules/$(shell uname -r)/build

obj-m := $(NAME).o
$(NAME)-objs := main.o keyboard.o logger.o utils.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	-@rmmod $(NAME) 2>/dev/null || true

run: all
	insmod $(NAME).ko

re: clean all
