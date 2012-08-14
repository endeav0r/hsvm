CC=gcc
CFLAGS=-Wall -O2 -g
LIBS=-lreadline

_OBJS = instruction.o lexer.o token.o vm.o

SRCDIR = src
OBJS = $(patsubst %,$(SRCDIR)/%,$(_OBJS))

all : assembler hsvm disassembler debugger

assembler : $(OBJS) $(SRCDIR)/assembler.o
	$(CC) $(CFLAGS) $(LIBS) -o assembler $(SRCDIR)/assembler.o $(OBJS)

hsvm : $(OBJS) $(SRCDIR)/hsvm.o
	$(CC) $(CFLAGS) $(LIBS) -o hsvm $(SRCDIR)/hsvm.o $(OBJS)

disassembler : $(OBJS) $(SRCDIR)/disassembler.o
	$(CC) $(CFLAGS) $(LIBS) -o disassembler $(SRCDIR)/disassembler.o $(OBJS)

debugger : $(OBJS) $(SRCDIR)/debugger.o
	$(CC) $(CFLAGS) $(LIBS) -o debugger $(SRCDIR)/debugger.o $(OBJS)

%.o : %.cc %.h
	$(CC) -c -o $@ $< $(CFLAGS)

%.o : %.cc
	$(CC) -c -o $@ $< $(CFLAGS)

clean :
	rm -f $(SRCDIR)/*.o
	rm -f assembler
	rm -f hsvm
	rm -f disassembler
	rm -f debugger