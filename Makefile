CC=gcc
CFLAGS=-Wall -O2 

_OBJS = instruction.o lexer.o token.o vm.o linenoise.o

SRCDIR = src
OBJS = $(patsubst %,$(SRCDIR)/%,$(_OBJS))

all : assembler hsvm disassembler debugger

assembler : $(OBJS) $(SRCDIR)/assembler.o
	$(CC) $(CFLAGS) -o assembler $(SRCDIR)/assembler.o $(OBJS)

hsvm : $(OBJS) $(SRCDIR)/hsvm.o
	$(CC) $(CFLAGS) -o hsvm $(SRCDIR)/hsvm.o $(OBJS)

disassembler : $(OBJS) $(SRCDIR)/disassembler.o
	$(CC) $(CFLAGS) -o disassembler $(SRCDIR)/disassembler.o $(OBJS)

debugger : $(OBJS) $(SRCDIR)/debugger.o
	$(CC) $(CFLAGS) -o debugger $(SRCDIR)/debugger.o $(OBJS)

windows : src/instruction.o src/lexer.o src/token.o src/vm.o src/assembler.o src/hsvm.o src/disassembler.o
	$(CC) $(CFLAGS) -o assembler.exe src/token.o src/instruction.o src/lexer.o src/assembler.o -lws2_32 
	$(CC) $(CFLAGS) -o disassembler.exe src/instruction.o src/disassembler.o -lws2_32 
	$(CC) $(CFLAGS) -o hsvm.exe src/instruction.o src/vm.o src/hsvm.o -lws2_32 

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
	rm -f assembler.exe
	rm -f hsvm.exe
	rm -f disassembler.exe
