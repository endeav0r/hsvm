; we have a stack canary but it does not really matter as there is no return
; pointer on the stack, or really anything in the high stack addresses

mov rbp, rsp
jmp main

str_enter_password : "Enter Password: "
str_you_entered    : "You Entered: "
str_newline        : "\n"
str_accepted       : "Password Accepted\n"
str_denied         : "Password Denied\n"
str_password       : "sigsac2013thiswillchange"
str_canary_error   : "Stack canary was corrupted\n"

main :
    sub rsp, 0x24
    ; rbp-0x02 = canary value
    ; rbp-0x04 = address of buffer
    ; rbp-0x24 = 32 byte buffer
    
    ; calculate buffer address
    mov r2, rbp
    sub r2, 0x24 ; r2 = address of input string

    ; store buffer address
    mov  r3, rbp
    sub  r3, 0x4 ; r3 = stack location storing address of buffer
    stor r3, r2

main_loop :

    ; set canary
    mov  r0, 0x000a
    mov  r1, rbp
    sub  r1, 2
    stor r1, r0

    push str_enter_password
    call puts
    add rsp, 2

    push r2
    call gets
    add rsp, 2

    cmp r0, 0xffff
    je eof

    ; check canary
    mov  r0, 0x000a
    mov  r1, rbp
    sub  r1, 2
    load r1, r1
    cmp  r1, r0
    jne  canary_fail

    ; get address of string
    load r2, r3

    push str_you_entered
    call puts
    push r2
    call puts
    push str_newline
    call puts
    add rsp, 6

    push r2
    push str_password
    call strcmp
    add rsp, 4
    cmp r0, 0
    je accept_password

    push str_denied
    call puts
    add rsp, 2
    jmp main_loop

accept_password :
    push str_accepted
    call puts
    hlt

canary_fail :
    push str_canary_error
    call puts
    hlt

eof :
    hlt
