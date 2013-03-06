

CC = gcc
CFLAGS = -g3 -O0 --std=gnu11 -fPIC

TARGETS = libplugin.so foo.so unplug

all : $(TARGETS)

unplug : unplug.o
	$(CC) $(CFLAGS) -o $@ $< -ldl -lelf -Wl,-rpath,/home/pjones/devel/local/unplug -L. -lplugin
	
# -Wl,--wrap,open

libplugin.so : plugin.o
	$(CC) $(CFLAGS) --shared -o $@ $<

foo.so : foo.o
	$(CC) $(CFLAGS) --shared -o $@ $< -L. -lplugin

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.E : %.c
	$(CC) $(CFLAGS) -E -o $@ $<

%.s : %.c
	$(CC) $(CFLAGS) -s -o $@ $<

clean :
	@rm -vf $(TARGETS) *.o *.so *.E
