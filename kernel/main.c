#include "keyboard.h"
#include "screen.h"
#include "memory.h"
#include "../drivers/vga.h"
#include "../filesystem/fs.h"
#include "../shell/shell.h"

void kernel_main() {
    init_screen();
    init_keyboard();
    init_memory();
    vga_init();
    init_filesystem();
    init_shell();
    
    print("Welcome to SimpleOS!\n");
    print("Type 'help' for a list of commands.\n");
    
    while(1) {
        handle_input();
    }
}

