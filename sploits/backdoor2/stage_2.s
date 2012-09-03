jmp begin

str_flag : "flag" ; will be at address 0x8084

begin :
	mov r1, 0x8084
	mov r2, 1
	mov r0, 0
	syscall

	mov r1, r0
	mov r2, 0x7000
	mov r3, 64
	mov r0, 1
	syscall
	; r0 = number of bytes actually read

	mov r4, 0
loop :
	cmp r4, r0
	je done
	cmp r4, 64
	je done
	add r1, r2, r4
	loadb r1, r1
	out r1
	add r4, 1
	jmp loop

done :
	hlt