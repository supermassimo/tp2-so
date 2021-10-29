include Makefile.inc

KERNEL=kernel.bin

SOURCES=$(wildcard *.c)
SOURCES_INTERRUPTIONS=$(wildcard interruptions/*.c)
SOURCES_IO=$(wildcard io/*.c)
SOURCES_HEAP=$(wildcard memory/heap.c)
SOURCES_BUDDY=$(wildcard memory/buddy.c)
SOURCES_MEM_MANAGER=$(wildcard memory/memManager.c)
SOURCES_INTERRUPTIONS_ASM=$(wildcard interruptions/*.asm)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_INTERRUPTIONS=$(SOURCES_INTERRUPTIONS:.c=.o)
OBJECTS_IO=$(SOURCES_IO:.c=.o)
OBJECTS_HEAP=$(SOURCES_HEAP:.c=.o)
OBJECTS_BUDDY=$(SOURCES_BUDDY:.c=.o)
OBJECTS_MEM_MANAGER=$(SOURCES_MEM_MANAGER:.c=.o)
OBJECTS_INTERRUPTIONS_ASM=$(SOURCES_INTERRUPTIONS_ASM:.asm=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

ALL_OBJECTS= $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTIONS) $(OBJECTS_INTERRUPTIONS_ASM) $(OBJECTS_IO) $(OBJECTS_MEM_MANAGER)

$(KERNEL): $(STATICLIBS) $(ALL_OBJECTS) $(OBJECTS_HEAP)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $^

buddy: $(STATICLIBS) $(ALL_OBJECTS) $(OBJECTS_BUDDY)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $^
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I./interruptions -I./io -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf $(ALL_OBJECTS) $(OBJECTS_HEAP) $(OBJECTS_BUDDY) *.bin

.PHONY: all buddy clean
