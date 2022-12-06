CC=gcc
CFLAGS=-Wall -extra -c -g -O0 -DDEBUG
LDFLAGS=


OUTDIR=bin

all: createdir compile link

compile:
	$(CC) $(CFLAGS) main.c -o $(OUTDIR)/asm.o

link:
	$(CC) $(LDFLAGS) $(OUTDIR)/asm.o -o $(OUTDIR)/asm

clean:
	rm -r -f $(OUTDIR)/

createdir:
	mkdir -p $(OUTDIR)
