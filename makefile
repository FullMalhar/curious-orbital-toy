# Makefile for Curious Orbital Toy

# Directories
INCDIR = ./include
SRCDIR = ./src

# Compilers
CC = gcc

# Flags
CFLAGS = -I$(INCDIR)
CFILES = $(shell find $(SRCDIR)/ -type f -name '*.c')
OBJS = $(patsubst %.c,%.o,$(CFILES))
LIBS = -lglfw

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: all clean

cot: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f $(shell find $(SRCDIR)/ -type f -name '*.o') ./cot