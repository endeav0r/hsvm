xor r1, r1, r1 ; r1 = 0
xor r1, 0x8080 ; r1 = 0x8080
xor r2, r2, r2 ; r2 = 0
add r2, 0x9090 ; r2 = 0x9090
xor r2, 0x9009 ; r2 = 0x0099

loop :
	in r3

	storb r1, r3

	sub r1, -1 ; r1 = r1 + 1
	
	cmp r3, r2
	jne loop

execute :
	mov rip, 0x8080