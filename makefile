###############################################################################
# Another very simple makefile. Run as one of:
# make
# make clean
# make debug
#
###############################################################################

CC = gcc
CCFLAGS = -ansi -Wall -pedantic
LIBS = -lm

all: ga

debug: CCFLAGS += -DDEBUG
debug: ga

ga:
	$(CC) $(CCFLAGS) -o ga *.c $(LIBS)

clean:
	rm ga
