jmp begin

str_flag : "flag" ; will be at address 0x2004

begin :
	mov r1, 0x2004
	mov r2, 1
	mov r0, 0
	syscall

	mov r1, r0
	mov r2, 0x7000
	mov r3, 64
	mov r0, 1
	syscall
	; r0 = number of bytes actually read

	mov r4, 0x7000
loop :
	cmp r0, 0
	je done

	loadb r3, r4
	out r3
	add r4, 1
	sub r0, 1
	jmp loop

done :
	hlt