strcpy :
    push rbp
    mov rbp, rsp
    push r1
    push r2
    push r3

    mov r0, rbp
    add r0, 4 ; arg0
    load r0, r0

    mov r1, rbp
    add r1, 6 ; arg1
    load r1, r1

    mov r3, r0 ; saved r0 so we can get bytes copied later

strcpy_loop :
    loadb r2, r1
    cmp r2, 0
    je strcpy_done

    storb r0, r2
    add r0, 1
    add r1, 1
    jmp strcpy_loop

strcpy_done :
    loadb r2, r1 ; copy over null
    storb r0, r2

    ; calculate length of final string
    sub r0, r0, r3

    pop r3
    pop r2
    pop r1
    pop rbp
    ret


strlen :
    push rbp
    mov rbp, rsp
    push r1
    push r2

    mov r0, rbp
    add r0, 4 ; arg0
    load r0, r0

    mov r1, r0

strlen_loop :
    loadb r2, r0
    cmp r2, 0
    je strlen_done
    add r0, 1
    jmp strlen_loop

strlen_done :
    sub r0, r0, r2
    
    pop r2
    pop r1
    pop rbp
    ret


strcmp :
    push rbp
    mov rbp, rsp
    push r1
    push r2
    push r3

    mov r0, rbp
    add r0, 4 ; arg0
    load r0, r0

    mov r1, rbp
    add r1, 6 ; arg1
    load r1, r1

strcmp_loop :
    loadb r2, r0
    loadb r3, r1

    cmp r2, r3
    jl strcmp_lt
    jg strcmp_gt

    cmp r2, 0
    je strcmp_eq

    add r0, 1
    add r1, 1
    jmp strcmp_loop

strcmp_lt :
    mov r0, -1
    jmp strcmp_ret

strcmp_gt :
    mov r0, 1
    jmp strcmp_ret

strcmp_eq :
    mov r0, 0
    jmp strcmp_ret

strcmp_ret :
    pop r3
    pop r2
    pop r1
    pop rbp
    ret
