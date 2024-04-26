# Makefile for Curious Orbital Toy

# Directories
INCDIR = ./include
SRCDIR = ./src

# Files
CFILES = $(shell find $(SRCDIR)/ -type f -name '*.cpp')
HFILES = $(shell find $(INCDIR)/ -type f -name '*.h')

# Compilers
CC = g++

# Flags
CFLAGS = -I$(INCDIR)
OBJS = $(patsubst %.cpp,%.o,$(CFILES))
LIBS = -lm -lsfml-graphics -lsfml-window -lsfml-system -lfmt

%.o: %.cpp $(HFILES)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

cot: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f $(shell find $(SRCDIR)/ -type f -name '*.o')