IDIR =headers
CC=gcc
CFLAGS=-Wall -ansi -pedantic -I$(IDIR)

ODIR=objects

_DEPS = breaker.h cmdInfo.h compile.h data.h general.h linkedList.h processor.h symbol.h extra.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o breaker.o cmdInfo.o compile.o data.o linkedList.o processor.o symbol.o extra.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

assembler: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
