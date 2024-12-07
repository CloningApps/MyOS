[bits 32]
[extern main]
[extern handle_keyboard]

global _start
global keyboard_handler

_start:
    ; Set up paging
    mov eax, page_directory
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Set up IDT
    call setup_idt
    sti

    ; Jump to C main function
    call main

    jmp $

; IDT setup
setup_idt:
    lidt [idt_descriptor]
    ret

; Keyboard interrupt handler
keyboard_handler:
    pushad
    in al, 0x60
    push eax
    call handle_keyboard
    add esp, 4
    mov al, 0x20
    out 0x20, al
    popad
    iret

; IDT
align 4
idt:
    times 33 * 8 db 0  ; 33 null interrupt handlers

idt_descriptor:
    dw 33 * 8 - 1
    dd idt

; Page tables
align 4096
page_directory:
    times 1024 dd 0x00000002

page_table:
    %assign i 0
    %rep 1024
        dd (i << 12) | 3
        %assign i i+1
    %endrep

