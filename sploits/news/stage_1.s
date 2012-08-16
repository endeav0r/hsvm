mov r3, 0x8888 ; set r3 to 0x0099
xor r3, 0x8811
mov r1, 0x8080 ; we will write our payload to 0x8080
read_payload :
    in r2
    storb r1, r2 ; write payload byte
    sub r1, -1
    cmp r2, r3 ; if we haven't reached final byte, keep reading input,
               ; else go to stage 2
    jne read_payload
    mov rip, 0x8080 
