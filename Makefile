

CC = gcc
CFLAGS = -g3 -O0 --std=gnu11 -lelf

TARGETS = foo.so unplug

all : $(TARGETS)

unplug : unplug.o
	$(CC) $(CFLAGS) -o $@ $<

%.so : %.o
	$(CC) $(CFLAGS) --shared -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

foo.o : foo.c
	$(CC) -fPIC -c -o $@ $<

clean :
	@rm -rf $(TARGETS) *.o *.so
