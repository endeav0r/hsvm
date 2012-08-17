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
    mov   r1, 0
    storb r0, r1 
    sub   r0, r0, r2

    pop r2
    pop r1
    pop rbp
    ret



; reads string input up to N characters
; buffer[N-1] will always be null-terminated
; N must be > 0
; getns(char * buf, uint N)
getns :
    push rbp
    mov rbp, rsp

    push r1
    push r2

    mov r0, rbp
    add r0, 4
    load r0, r0 ; r0 = buffer to read into

    mov r1, rbp
    add r1, 6
    load r1, r1 ; r1 = number of bytes to read

    cmp r1, 0
    je getns_zero_n

getns_loop :
    sub r1, 1
    cmp r1, 0
    je getns_done

    in r2
    cmp r2, 0x00
    je getns_done
    cmp r2, 0x0a
    je getns_done
    cmp r2, 0xffff
    je getns_done

    storb r0, r2
    add r0, 1
    jmp getns_loop

getns_done :
    ; terminate string
    mov r2, 0
    storb r0, r2

    ; find number of bytes read
    mov r1, rbp
    add r1, 4
    load r1, r1
    sub r0, r0, r1

    jmp getns_return

getns_zero_n :
    mov r0, 0
    jmp getns_return

getns_return :
    pop r2
    pop r1
    pop rbp
    ret