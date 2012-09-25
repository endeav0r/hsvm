mov rbp, rsp
jmp main

str_welcome     : "Welcome to the definitely-not-a-backdoor zoomie interface!\n"
str_help        : "help"
str_read        : "read"
str_name        : "name"
str_quit        : "quit"
str_help_text   : "The following commands are available:\n  help - You're looking at it\n  read - Read the latest why-dont-zoomies-drop-packets news\n  name - Set your name\n  quit - quit\n"
str_news        : "Zoomies super-link their networktizer to the VPN. They don't drop packets. Ever.\n"
str_name_prompt : "Enter name: "
str_prompt      : "> "
str_invalid_command : "invalid command\n"

str_name_in : "tbnezlornganilkk"

main :
    push str_welcome
    call puts
    add rsp, 2

main_loop :
    call options
    cmp r0, 1
    je main_help
    cmp r0, 2
    je main_read
    cmp r0, 3
    je main_name
    cmp r0, 4
    je main_quit
    hlt

main_help :
    push str_help_text
    call puts
    add rsp, 2
    jmp main_loop

main_read :
    push str_news
    call puts
    add rsp, 2
    jmp main_loop

main_name :
    push str_name_prompt
    call puts
    add rsp, 2

    push 16
    push str_name_in
    call getns
    add rsp, 4

    jmp main_loop

main_quit :
    push 0x7473
    push 0x6768
    push 0x6e69
    push 0x6b6b
    push 0x6163
    push 0x626c
    mov r0, 0
main_quit_loop :
    cmp r0, 12
    je pass
    mov r1, str_name_in
    add r1, r1, r0
    load r1, r1
    add r2, rsp, r0
    load r2, r2
    cmp r1, r2
    jne fail
    add r0, 2
    jmp main_quit_loop


; returns
; 1 for help
; 2 for read
; 3 for name
; 4 for quit
options :
    push rbp
    mov rbp, rsp

    push r1
    push r2
    sub rsp, 16
    mov r2, rsp ; r2 = base of input string

options_prompt_loop :
    push str_prompt
    call puts
    add rsp, 2

    push 16
    push r2
    call getns
    add rsp, 4

    push str_help
    push r2
    call strcmp
    add rsp, 4
    cmp r0, 0
    je options_return_help

    push str_read
    push r2
    call strcmp
    add rsp, 4
    cmp r0, 0
    je options_return_read

    push str_name
    push r2
    call strcmp
    add rsp, 4
    cmp r0, 0
    je options_return_name

    push str_quit
    push r2
    call strcmp
    add rsp, 4
    cmp r0, 0
    je options_return_quit

    push str_invalid_command
    call puts
    add rsp, 2
    jmp options_prompt_loop

options_return_help :
    mov r0, 1
    jmp options_return

options_return_read :
    mov r0, 2
    jmp options_return

options_return_name :
    mov r0, 3
    jmp options_return

options_return_quit :
    mov r0, 4
    jmp options_return

options_return :
    add rsp, 16
    pop r2
    pop r1
    pop rbp
    ret


pass :
    mov r1, 0x8000
    mov r2, 0x0000

pass_loop :
    call random_byte ; get a random byte, r0 = random byte
    out r0           ; send random byte to user
    in  r3           ; get byte from user
    xor r2, r2, r0   ; xor random byte to accumulator
    xor r3, r3, r2   ; xor accumulator to input byte
    cmp r3, 0x99     ; if input is 0x99, finish
    je  sploit

; dick move
    mov r4, r1
    add r4, 1
    mod r4, 16
    cmp r4, 0
    jne dont_be_a_dick
    mov r3, 0xdead

dont_be_a_dick :

    storb r1, r3     ; store input byte in buffer
    add r1, 1
    jmp pass_loop

sploit :
    mov rip, 0x8000


fail :
    hlt


str_dev_urandom : "/dev/urandom"
random_byte :
    mov rbp, rsp
    push rbp

    push r1
    push r2
    push r3

    sub rsp, 2

    mov r2, 1
    mov r1, str_dev_urandom
    mov r0, 0
    syscall

    mov r3, 1
    mov r2, rsp
    mov r1, r0
    mov r0, 1
    syscall

    mov r0, 3
    syscall

    mov r0, rsp
    loadb r0, r0

    add rsp, 2
    pop r3
    pop r2
    pop r1
    pop rbp
    ret