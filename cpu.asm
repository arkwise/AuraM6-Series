; cpu monitor
; nasm -f bin src/xapps/cpu.asm -o bin/cpu.com to compile it.
;
[ORG 0x0100]     ; Origin, standard for .COM programs
[bits 16]       ; 16-bit code

section .data
    oldInt     dw 0      ; Old interrupt handler offset
    oldIntSeg  dw 0      ; Old interrupt handler segment

section .text

start:
mov al, [es:82h]       ; Second character of command line

    ; Restore old interrupt handler
    cli                    ; Clear interrupts
    mov ax, 0x251C         ; AH = 25h (Set Interrupt Vector), 
                           ; AL = 1Ch (our chosen interrupt)
    lea dx, [oldInt]       ; Address of old interrupt handler
    int 0x21               ; DOS interrupt
    sti                    ; Restore interrupts

    ; Terminate program
    mov ax, 0x4C00
    int 0x21               ; DOS interrupt

notUnloading:
    ; Install our custom interrupt handler
    cli                    ; Clear interrupts
    mov ax, 0x351C         ; AH = 35h (Get Interrupt Vector), 
                           ; AL = 1Ch (our chosen interrupt)
    int 0x21               ; DOS interrupt
    mov [oldInt], bx       ; Save old offset
    mov [oldIntSeg], es    ; Save old segment

    ; Set our new interrupt handler
    mov ax, 0x251C         ; AH = 25h (Set Interrupt Vector), 
                           ; AL = 1Ch (our chosen interrupt)
    lea dx, [newInterrupt]
   ; Address of our interrupt handler
    int 0x21               ; DOS interrupt

    ; Terminate and stay resident
    mov ax, 0x3100         ; AH = 31h (Terminate and Stay Resident)
    int 0x21               ; DOS interrupt

newInterrupt:
    ; Our custom interrupt handler
    ; For demonstration purposes, just return ax = 1234
    mov ax, 1234           ; Sample value

    ; Jump to old interrupt (chain to it)
    push ds
    mov dx, [oldInt]
    mov ds, [oldIntSeg]
    db 0xFF                ; CALL FAR instruction opcode
    dw oldInt
    dw oldIntSeg
    pop ds
    iret

section .end
