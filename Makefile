CC=gcc
CFLAGS = -Wall -O2 -g

_OBJS = instruction.o lexer.o token.o linenoise.o

SRCDIR = src
OBJS   = $(patsubst %,$(SRCDIR)/%,$(_OBJS))

VMOBJ      = $(SRCDIR)/vm.o
VMDEBUGOBJ = $(SRCDIR)/vmdebug.o

all : assembler hsvm disassembler debugger

assembler : $(OBJS) $(SRCDIR)/assembler.o vm
	$(CC) $(CFLAGS) -o assembler $(SRCDIR)/assembler.o $(OBJS) $(VMOBJ)

hsvm : $(OBJS) $(SRCDIR)/hsvm.o vm
	$(CC) $(CFLAGS) -o hsvm $(SRCDIR)/hsvm.o $(OBJS) $(VMOBJ)

disassembler : $(OBJS) $(SRCDIR)/disassembler.o vm
	$(CC) $(CFLAGS) -o disassembler $(SRCDIR)/disassembler.o $(OBJS) $(VMOBJ)

debugger : $(OBJS) $(SRCDIR)/debugger.o debug_vm
	$(CC) $(CFLAGS) -o debugger $(SRCDIR)/debugger.o $(OBJS) $(VMDEBUGOBJ)

windows : src/instruction.o src/lexer.o src/token.o src/vm.o src/assembler.o src/hsvm.o src/disassembler.o
	$(CC) $(CFLAGS) -o assembler.exe src/token.o src/instruction.o src/lexer.o src/assembler.o -lws2_32 
	$(CC) $(CFLAGS) -o disassembler.exe src/instruction.o src/disassembler.o -lws2_32 
	$(CC) $(CFLAGS) -o hsvm.exe src/instruction.o src/vm.o src/hsvm.o -lws2_32 

%.o : %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)

debug_vm : $(SRCDIR)/vm.c $(SRCDIR)/vm.h
	$(CC) -c -o $(SRCDIR)/vmdebug.o $(SRCDIR)/vm.c -DDEBUG_GETCHAR $(CFLAGS)

vm : $(SRCDIR)/vm.c $(SRCDIR)/vm.h
	$(CC) -c -o $(SRCDIR)/vm.o $(SRCDIR)/vm.c $(CFLAGS)

clean :
	rm -f $(SRCDIR)/*.o
	rm -f assembler
	rm -f hsvm
	rm -f disassembler
	rm -f debugger
	rm -f assembler.exe
	rm -f hsvm.exe
	rm -f disassembler.exe
