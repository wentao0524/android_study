CC      = mipsel-openwrt-linux-gcc
LD      = mipsel-openwrt-linux-ld
#CC      = gcc
#LD      = ld

#AR      = arm-linux-ar
#OBJCOPY = arm-linux-objcopy
#OBJDUMP = arm-linux-objdump

#INCLUDEDIR 	:= $(shell pwd)/include  
CFLAGS 		:= -Wall -g 
#CFLAGS 		:= -Wall -g -fno-builtin
#CPPFLAGS   	:= -nostdinc 
CPPFLAGS   	:= 
#CPPFLAGS   	:= -nostdinc  -I/usr/include  -I/usr/include/linux -I/usr/include/c++/4.9
#-I$(INCLUDEDIR) -I$(shell pwd)/jpeg_lib -I$(shell pwd)/hardware
#LDFLAGS 	:= -lgcc
#LDPATH      := -L/usr/local/arm/4.5.1/lib/gcc/arm-none-linux-gnueabi/4.5.1

export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

objs := socket_base.o   client.o  uart.o

client: $(objs)
	${CC} -o bishe.elf $^ -lc

%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.bin *.elf *.dis *.o *~
	
