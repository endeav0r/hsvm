mov rbp, rsp
jmp main

str_goodbye : "goodbye\n"

main :
	in r0
	cmp r0, 107 ; k
	jne goodbye
	in r0
	cmp r0, 110 ; n
	jne goodbye
	in r0
	cmp r0, 111 ; o
	jne goodbye
	in r0
	cmp r0, 99  ; c
	jne goodbye
	in r0
	cmp r0, 107 ; k
	jne goodbye

	mov r0, 0x8000
	mov r1, 0

loop :
	in r3

	xor r3, 0x99
	
	cmp r3, 0x00
	je execute
	cmp r3, 0x0a
	je execute

	add r2, r1, r0
	storb r2, r3

	add r1, 1
	jmp loop

execute :
	mov rip, 0x8000

goodbye :
	push str_goodbye
	call puts
	hlt