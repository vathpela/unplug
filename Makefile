

CC = gcc
CFLAGS = -g3 -O0

TARGETS = foo.o unplug

all : $(TARGETS)

unplug : unplug.o
	$(CC) $(CFLAGS) -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<
