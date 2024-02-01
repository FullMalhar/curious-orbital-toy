# Makefile for Curious Orbital Toy

# Directories
INCDIR = ./include
SRCDIR = ./src

# Files
CFILES = $(shell find $(SRCDIR)/ -type f -name '*.c')
HFILES = $(shell find $(INCDIR)/ -type f -name '*.h')

# Compilers
CC = gcc

# Flags
CFLAGS = -I$(INCDIR)
OBJS = $(patsubst %.c,%.o,$(CFILES))
LIBS = -lglfw

%.o: %.c $(HFILES)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: all clean

cot: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f $(shell find $(SRCDIR)/ -type f -name '*.o') ./cot