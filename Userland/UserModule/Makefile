include ../Makefile.inc

MODULE=0000-userModule.bin
SOURCES=$(wildcard [^_]*.c)
LIB=standard_library
LIB_SOURCES=$(wildcard ./standard_library/*.c)
LIB_SOURCES_ASM=$(wildcard ./standard_library/*.asm)
LIB_OBJECTS=$(LIB_SOURCES:.c=.o)
LIB_OBJECTS_ASM=$(LIB_SOURCES_ASM:.asm=.o)
LIB_INC_DIR=./standard_library/include

ALL_LIB_OBJECTS=$(LIB_OBJECTS) $(LIB_OBJECTS_ASM)

all: $(MODULE)

$(MODULE): $(SOURCES) $(ALL_LIB_OBJECTS)
	$(GCC) $(GCCFLAGS) -T userModule.ld _loader.c $(SOURCES) -o ../$(MODULE) $(ALL_LIB_OBJECTS)
	$(GCC) $(GCCFLAGS) -T userModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(SOURCES) -o ../0000-userModule.elf $(ALL_LIB_OBJECTS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I$(LIB_INC_DIR) -lstring -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf $(LIB)/*.o

.PHONY: all clean print
