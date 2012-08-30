mov rbp, rsp
jmp main

str_helloworld : "Hello World!\n"

main :
    mov r0, str_helloworld

loop :
    loadb r1, r0
    cmp r1, 0
    je done

    out r1
    add r0, 1
    jmp loop

done :
    hlt
