#include <stdint.h>
#include <stdbool.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define IDT_SIZE 256
#define KEYBOARD_IRQ 33

struct IDT_entry {
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

volatile char *video = (volatile char*)0xb8000;
int current_loc = 0;
char current_command[256];
int command_index = 0;

void idt_init(void) {
    extern int keyboard_handler();
    uint64_t keyboard_address;
    uint64_t idt_address;
    uint64_t idt_ptr[2];

    keyboard_address = (uint64_t)keyboard_handler;
    IDT[KEYBOARD_IRQ].offset_lowerbits = keyboard_address & 0xffff;
    IDT[KEYBOARD_IRQ].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET
    IDT[KEYBOARD_IRQ].zero = 0;
    IDT[KEYBOARD_IRQ].type_attr = 0x8e; // INTERRUPT_GATE
    IDT[KEYBOARD_IRQ].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    // Load IDT
    idt_address = (uint64_t)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    __asm__ __volatile__("lidtq (%0)" : : "r" (idt_ptr));
}

void kb_init(void) {
    __asm__ __volatile__("outb %%al, %%dx" : : "a"(0xFD), "d"(0x21));
}

void print(const char *str) {
    while(*str != 0) {
        video[current_loc++] = *str++;
        video[current_loc++] = 0x07;
    }
}

void clear_screen() {
    int i;
    for (i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }
    current_loc = 0;
}

void new_line() {
    current_loc = (current_loc / (2 * VGA_WIDTH) + 1) * 2 * VGA_WIDTH;
}

void backspace() {
    if (current_loc > 0) {
        current_loc -= 2;
        video[current_loc] = ' ';
    }
}

void execute_command() {
    if (strcmp(current_command, "clear") == 0) {
        clear_screen();
    } else if (strcmp(current_command, "echo") == 0) {
        print("Echo command received");
    } else if (strcmp(current_command, "ls") == 0) {
        print("File1.txt File2.txt Directory1/");
    } else if (strcmp(current_command, "apt-get") == 0) {
        print("Package manager: apt-get command received");
    } else if (strcmp(current_command, "dir") == 0) {
        print("File1.txt File2.txt Directory1\\");
    } else {
        print("Unknown command: ");
        print(current_command);
    }
    new_line();
    print("SimpleOS> ");
    command_index = 0;
    current_command[0] = '\0';
}

void handle_keyboard(uint8_t scancode) {
    if (scancode == 0x1C) {  // Enter key
        new_line();
        execute_command();
    } else if (scancode == 0x0E) {  // Backspace
        if (command_index > 0) {
            command_index--;
            current_command[command_index] = '\0';
            backspace();
        }
    } else {
        char c = scancode_to_char(scancode);
        if (c) {
            current_command[command_index++] = c;
            current_command[command_index] = '\0';
            video[current_loc++] = c;
            video[current_loc++] = 0x07;
        }
    }
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

char scancode_to_char(uint8_t scancode) {
    const char scancode_to_ascii[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
        0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' '
    };

    if (scancode < sizeof(scancode_to_ascii) / sizeof(char)) {
        return scancode_to_ascii[scancode];
    }
    return 0;
}

void main(void) {
    idt_init();
    kb_init();

    clear_screen();
    print("Welcome to SimpleOS!\n");
    print("SimpleOS> ");

    while(1) {
        __asm__ __volatile__("hlt");
    }
}

