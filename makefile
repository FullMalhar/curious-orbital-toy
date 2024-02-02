# Makefile for Curious Orbital Toy

# Directories
CSFDIR = ./../CSFML
INCDIR = ./include
SRCDIR = ./src
OUTDIR = ./build

# Files
CFILES = $(shell find $(SRCDIR)/ -type f -name '*.c')
HFILES = $(shell find $(INCDIR)/ -type f -name '*.h') 
LFILES = $(CSFDIR)/lib/libcsfml-window.so.2.5

# Compilers
CC = gcc

# Flags
CFLAGS = -I$(INCDIR) -I$(CSFDIR)/include
LFLAGS = -L$(CSFDIR)/lib -Wl,-rpath '-Wl,$$ORIGIN'
OBJS = $(patsubst %.c,%.o,$(CFILES)) $(patsubst %.cpp,%.o,$(CPPFILES))
LIBS = -lglfw -lm -lcsfml-window

%.o: %.c $(HFILES)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

cot: $(OBJS)
	$(CC) -o $(OUTDIR)/$@ $^ $(LFLAGS) $(LIBS)
	cp -t $(OUTDIR)/ $(LFILES)

clean:
	rm -f $(shell find $(SRCDIR)/ -type f -name '*.o') $(OUTDIR)/cot $(OUTDIR)/*.so*