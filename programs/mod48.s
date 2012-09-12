mov rbp, rsp
jmp main

str_prompt : "Enter string to mod 48: "
str_canary_fail : "Canary Fail\n"

main :
	call mod64
	hlt


mod64 :
	push rbp
	mov rbp, rsp
	push 0x000a ; set stack canary

	push r1
	push r2
	push r3
	push r5
	push r6
	push r7

	; prompt for input
	push str_prompt
	call puts
	add rsp, 2

	; get input
	push 0x100
	push 0x8000
	call input_read
	add rsp, 4
	mov r7, r0 ; r7 = bytes_read from input_read

	; strlen input
	push 0x8000
	call strlen
	add rsp, 2
	mov r6, r0 ; r6 = strlen of input

	; alloca
	sub rsp, rsp, r6
	mov r5, rsp ; r5 = location of alloca_buf

	; memcpy
	push r7
	push 0x8000
	push r5
	call memcpy
	add rsp, 6

	; check stack canary
	mov r1, rbp
	sub r1, 2
	load r1, r1
	cmp r1, 0x000a
	jne canary_fail

	; zero input_area
	push r7
	push 0
	push 0x8000
	call memset
	add rsp, 6

	; mod alloca_buf by 0x30
	mov r1, r5
	mov r2, r7
mod64_loop :
	cmp   r2, 0
	je    mod64_loop_done
	loadb r3, r1
	mod   r3, 0x30
	storb r1, r3
	add   r3, 1
	sub   r2, 1
	jmp   mod64_loop

mod64_loop_done :

	; output bytes
	mov r1, r5
	mov r2, r6
mod64_loop2 :
	cmp r2, 0
	je mod64_loop2_done
	loadb r3, r1
	out r3
	add r3, 1
	sub r2, 1
	jmp mod64_loop2

mod64_loop2_done :

	; unalloca
	add rsp, rsp, r6

	; ret
	pop r7
	pop r6
	pop r5
	pop r3
	pop r2
	pop r1
	pop rbp ; take off stack canary
	pop rbp
	ret



canary_fail :
	push str_canary_fail
	call puts
	hlt


memcpy :
	push rbp
	mov rbp, rsp

	push r1
	push r2
	push r3
	push r4

	mov  r1, rbp
	add  r1, 4
	load r1, r1
	mov  r0, r1

	mov  r2, rbp
	add  r2, 6
	load r2, r2

	mov  r3, rbp
	add  r3, 8
	load r3, r3

memcpy_loop :
	cmp r3, 0
	je memcpy_ret

	loadb r4, r2
	storb r1, r4

	add r1, 1
	add r2, 1
	sub r3, 1
	jmp memcpy_loop

memcpy_ret :

	pop r4
	pop r3
	pop r2
	pop r1
	pop rbp

	ret


memset :
	push rbp
	mov rbp, rsp

	push r1
	push r2
	push r3

	mov  r1, rbp
	add  r1, 4
	load r1, r1
	mov  r0, r1

	mov  r2, rbp
	add  r2, 6
	load r2, r2

	mov  r3, rbp
	add  r3, 8
	load r3, r3

memset_loop :
	cmp r3, 0
	je memset_ret

	storb r1, r2
	add   r1, 1
	sub   r3, 1

	jmp memset_loop

memset_ret :
	pop r3
	pop r2
	pop r1
	pop rbp

	ret



; void input_read(uint8 * buf_addr, uint16 size)
input_read :
	push rbp
	mov rbp, rsp
	push r1
	push r2
	push r3

	mov r2, rbp
	add r2, 4
	load r2, r2

	mov r3,  rbp
	add r3,  6
	load r3, r3

	mov r1, 0
	mov r0, 1
	syscall

	pop r3
	pop r2
	pop r1
	pop rbp

	ret