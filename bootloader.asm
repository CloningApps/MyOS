[org 0x7c00]
[bits 16]

; Set up stack
mov bp, 0x9000
mov sp, bp

; Load kernel from disk
mov bx, 0x1000  ; Load kernel to 0x1000
mov dh, 2       ; Load 2 sectors
mov dl, [BOOT_DRIVE]
call disk_load

; Switch to protected mode
cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 0x1
mov cr0, eax
jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call 0x1000  ; Jump to kernel code

; GDT
gdt_start:
    dq 0x0
gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Disk load function
disk_load:
    pusha
    push dx
    mov ah, 0x02
    mov al, dh
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    int 0x13
    jc disk_error
    pop dx
    cmp al, dh
    jne sectors_error
    popa
    ret

disk_error:
    mov bx, DISK_ERROR
    call print
    jmp $

sectors_error:
    mov bx, SECTORS_ERROR
    call print
    jmp $

; Print function (16-bit mode)
print:
    pusha
    mov ah, 0x0e
.loop:
    mov al, [bx]
    cmp al, 0
    je .done
    int 0x10
    inc bx
    jmp .loop
.done:
    popa
    ret

BOOT_DRIVE db 0
DISK_ERROR db "Disk read error", 0
SECTORS_ERROR db "Incorrect number of sectors read", 0

times 510-($-$$) db 0
dw 0xaa55

