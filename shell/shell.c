#include "shell.h"
#include "../kernel/screen.h"

void init_shell() {
    // Shell initialization code
}

void execute_command(const char* command) {
    if (strcmp(command, "help") == 0) {
        print("Available commands: help, clear, echo, ls\n");
    } else if (strcmp(command, "clear") == 0) {
        clear_screen();
    } else if (strcmp(command, "echo") == 0) {
        print("Echo command received\n");
    } else if (strcmp(command, "ls") == 0) {
        print("File1.txt File2.txt Directory1/\n");
    } else {
        print("Unknown command. Type 'help' for a list of commands.\n");
    }
}

