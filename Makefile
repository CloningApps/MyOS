# Compiler and flags
ASM = nasm
CC = gcc
LD = ld
ASMFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -c

# Directories
SRC_DIRS = bootloader.asm filesystem drivers kernel kernel.c lib memory shell
OBJ_DIR = obj

# Create object files from source files
ASM_SOURCES = $(wildcard $(SRC_DIRS)/*.asm)
C_SOURCES = $(wildcard $(SRC_DIRS)/*.c)

ASM_OBJECTS = $(ASM_SOURCES:.asm=.o)
C_OBJECTS = $(C_SOURCES:.c=.o)

# Final output
OUTPUT = myos.bin

# Build all object files
all: $(OUTPUT)

$(OUTPUT): $(ASM_OBJECTS) $(C_OBJECTS)
	$(LD) -m elf_i386 -o $(OUTPUT) $(ASM_OBJECTS) $(C_OBJECTS)

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up
clean:
	rm -f $(ASM_OBJECTS) $(C_OBJECTS) $(OUTPUT)
