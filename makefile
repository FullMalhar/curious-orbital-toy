# Makefile for Curious Orbital Toy

# Directories
INCDIR = ./include
SRCDIR = ./src
OUTDIR = ./build

# Files
CFILES = $(shell find $(SRCDIR)/ -type f -name '*.c')
HFILES = $(shell find $(INCDIR)/ -type f -name '*.h')

# Compilers
CC = gcc

# Flags
CFLAGS = -I$(INCDIR)
LFLAGS = -L$(OUTDIR) -Wl,-rpath '-Wl,$$ORIGIN'
OBJS = $(patsubst %.c,%.o,$(CFILES))
LIBS = -lglfw -lm -lcsfml-graphics

%.o: %.c $(HFILES)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

cot: $(OBJS)
	$(CC) -o $(OUTDIR)/$@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(shell find $(SRCDIR)/ -type f -name '*.o')