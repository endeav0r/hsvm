puts :
    push rbp
    mov rbp, rsp
    push r1

    mov r0, rbp
    add r0, 4 ; arg0
    load r0, r0

puts_loop :
    loadb r1, r0
    cmp r1, 0
    je puts_done

    out r1
    add r0, 1
    jmp puts_loop

puts_done :
    sub r0, r1, r0

    pop r1
    pop rbp
    ret



gets :
    push rbp
    mov rbp, rsp
    push r1
    push r2

    mov r0, rbp
    add r0, 4 ; arg0
    load r0, r0

    mov r2, r0

gets_loop :
    in r1
    cmp r1, 0x0a
    je gets_done
    cmp r1, 0x00
    je gets_done
    cmp r1, 0xffff
    je gets_done

    storb r0, r1
    add r0, 1
    jmp gets_loop

gets_done :
    ; null-terminate destination string
    mov r1, 0
    storb r0, r1 
    sub r0, r0, r2

    pop r2
    pop r1
    pop rbp
    ret

