
CROSS=arm-linux-
CC=$(CROSS)gcc
LD=$(CROSS)ld
AR=$(CROSS)ar
#export CROSS CC AR LD

source:=$(wildcard *.c)
objs=$(source:.c=.o)


target= bd_test
    
all: $(target)

bd_test:$(objs)
	$(CC) -static -o $@ $(objs) -lpthread
	
%.o: %.c
	$(CC) $(CFLAGS) -c -fPIC "$<" $(CFG_INC)

clean:
	-rm -f $(target)
	-rm -f *.o


