mov r3, 0x8888 ; set r3 to 0x0099
xor r3, 0x8811
mov r1, 0x8080 ; we will write our payload to 0x8080
read_payload :
    in r2
    cmp r2, r3 ; when we read 0x99, jump to payload
    mov rip, 0x8080 
    storb r1, r2 ; write payload byte
    sub r1, -1
    jmp read_payload
