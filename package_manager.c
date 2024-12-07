#include <stdint.h>
#include <stdbool.h>

#define MAX_PACKAGES 100

struct Package {
    char name[64];
    char version[16];
    bool installed;
};

struct Package package_list[MAX_PACKAGES];
int package_count = 0;

void init_package_manager() {
    // In a real system, this would load package info from a file or database
    add_package("vim", "8.2", false);
    add_package("gcc", "10.2", false);
    add_package("python", "3.9", false);
}

void add_package(const char* name, const char* version, bool installed) {
    if (package_count < MAX_PACKAGES) {
        strcpy(package_list[package_count].name, name);
        strcpy(package_list[package_count].version, version);
        package_list[package_count].installed = installed;
        package_count++;
    }
}

void list_packages() {
    for (int i = 0; i < package_count; i++) {
        print(package_list[i].name);
        print(" ");
        print(package_list[i].version);
        print(package_list[i].installed ? " [Installed]" : " [Not Installed]");
        new_line();
    }
}

void install_package(const char* name) {
    for (int i = 0; i < package_count; i++) {
        if (strcmp(package_list[i].name, name) == 0) {
            if (package_list[i].installed) {
                print("Package already installed");
            } else {
                // In a real system, this would download and install the package
                package_list[i].installed = true;
                print("Package installed successfully");
            }
            return;
        }
    }
    print("Package not found");
}

// Add this to the execute_command function in kernel.c
void execute_command() {
    if (strcmp(current_command, "apt-get list") == 0) {
        list_packages();
    } else if (strncmp(current_command, "apt-get install ", 16) == 0) {
        install_package(current_command + 16);
    } else {
        // ... (other commands)
    }
}

