mov rbp, rsp
jmp main

prog0: 0x8888
prog1: 0x6488
prog2: 0x89ca
prog3: 0x888a
prog4: 0x988
prog5: 0x8b0a
prog6: 0x888c
prog7: 0xa088
prog8: 0x8d48
prog9: 0x888e
prog10: 0x8c88
prog11: 0x8f1a
prog12: 0x8880
prog13: 0x8088
prog14: 0x81ff
prog15: 0x8882
prog16: 0x8988
prog17: 0x8388
prog18: 0x8884
prog19: 0x8888
prog20: 0x8588
prog21: 0x8083
prog22: 0x8881
prog23: 0x8785
prog24: 0x8985
prog25: 0x828b
prog26: 0x8781
prog27: 0x8981
prog28: 0x828c
prog29: 0x8783
prog30: 0x8d89
prog31: 0x888e
prog32: 0x8c88
prog33: 0x8a80
prog34: 0x828c
prog35: 0x8084
prog36: 0x8d8a
prog37: 0x888e
prog38: 0x7c88
prog39: 0x8f84
prog40: 0x888f
prog41: 0x8288

str_pass : "PASS\n"
str_fail : "FAIL\n"

main :
	push 84
	push prog0
	call unpack
	call interpret

	cmp r0, 0
	je main_fail

	push str_pass
	call puts
	hlt

main_fail :
	push str_fail
	call puts
	hlt


; void unpack (void * base_of_data, int data_len)
unpack :
	push rbp
	mov rbp, rsp

	push r1
	push r2

	mov r0, rbp
	add r0, 4
	load r0, r0 ; r0 = pointer to next byte

	mov r1, rbp
	add r1, 6
	load r1, r1 ; r1 = data_len

unpack_loop :
	cmp r1, 0
	je unpack_done

	loadb r2, r0
	xor r2, 0x88
	storb r0, r2

	add r0, 1
	sub r1, 1
	jmp unpack_loop

unpack_done :
	pop r2
	pop r1
	pop rbp
	ret


interpret_load_operands :
	mov r1, r7
	add r1, 1
	loadb r1, r1 ; r1 = value of operand 0

	mov r3, rbp
	sub r3, 0x10
	add r3, r3, r1 ; r3 = address of operand 0

	mov r4, r7
	add r4, 2
	loadb r4, r4 ; r4 = value of operand 1

	mov r5, rbp
	sub r5, 0x10
	add r5, r5, r4 ; r5 = address of operand 1

	ret


interpret_sign_extend_byte :
	mov r0, rsp
	add r0, 2
	load r0, r0
	cmp r0, 0x80
	jl interpret_sign_extend_no

	or r0, 0xff00
	ret

interpret_sign_extend_no :
	ret


; int interpret (void * base_of_program)
interpret :
	push rbp
	mov rbp, rsp

	; rbp - 0x10 = base of program memory
	; rbp - 0x12 = base of program
	; rbp - 0x14 = program counter
	; rbp - 0x16 = "flags register"

	sub rsp, 0x16

	push r1
	push r2 ; = program counter
	push r3
	push r4
	push r5
	push r6
	push r7 

	; set base of program
	mov  r0, rbp
	add  r0, 4
	load r0, r0

	mov  r1, rbp
	sub  r1, 0x12
	stor r1, r0

	; set program counter
	mov  r2, 0
	mov  r1, rbp
	sub  r1, 0x14
	stor r1, r2

interpret_loop :
	; load instruction address into r0
	mov r0, rbp
	sub r0, 0x12
	load r0, r0

	mov r1, r2
	mul r1, 3
	add r7, r0, r1 ; r7 = address of instruction

	loadb r1, r7 ; r1 = instruction operand

	cmp r1, 0
	je interpret_set
	cmp r1, 1
	je interpret_add
	cmp r1, 2
	je interpret_sub
	cmp r1, 3
	je interpret_xor
	cmp r1, 4
	je interpret_cmp
	cmp r1, 5
	je interpret_je
	cmp r1, 6
	je interpret_jmp
	cmp r1, 7
	je interpret_ret
	cmp r1, 8
	je interpret_in
	cmp r1, 9
	je interpret_lod

interpret_loop_2 :
	add r2, 1
	jmp interpret_loop

interpret_set :
	call interpret_load_operands

	storb r3, r4

	jmp interpret_loop_2


interpret_add :
	call interpret_load_operands

	loadb r1, r3     ; r1 = [operand 0]
	loadb r4, r5     ; r4 = [operand 1]
	add   r1, r1, r4 ; r1 = [operand 0] + [operand 1]

	storb r3, r1

	jmp interpret_loop_2


interpret_sub :
	call interpret_load_operands

	loadb r1, r3     ; r1 = [operand 0]
	loadb r4, r5     ; r4 = [operand 1]
	sub   r1, r1, r4 ; r1 = [operand 0] + [operand 1]

	storb r3, r1

	jmp interpret_loop_2


interpret_xor :
	call interpret_load_operands

	loadb r1, r3     ; r1 = [operand 0]
	loadb r4, r5     ; r4 = [operand 1]
	xor   r1, r1, r4 ; r1 = [operand 0] + [operand 1]

	storb r3, r1

	jmp interpret_loop_2


interpret_cmp :
	call interpret_load_operands

	loadb r1, r3     ; r1 = [operand 0]
	loadb r4, r5     ; r4 = [operand 1]
	sub   r1, r1, r4 ; r1 = [operand 0] - [operand 1]

	mov r3, rbp
	sub r3, 0x16
	stor r3, r1

	jmp interpret_loop_2


interpret_je :
	call interpret_load_operands

	; r1 = jmp offset

	mov r3, rbp
	sub r3, 0x16
	load r3, r3

	cmp r3, 0
	jne interpret_loop_2

	; flags is equal, add r1 to pc (r2)
	; this will sign extend r1
	push r1
	call interpret_sign_extend_byte
	pop r1
	mov r1, r0

	add r2, r2, r1

	jmp interpret_loop_2


interpret_jmp :
	call interpret_load_operands

	; r1 = jmp offset

	; add r1 to pc (r2)
	; this will sign extend r1
	push r1
	call interpret_sign_extend_byte
	pop r1
	mov r1, r0

	add r2, r2, r1

	jmp interpret_loop_2


interpret_ret :
	call interpret_load_operands

	loadb r1, r3     ; r1 = [operand 0]
	mov r0, r1

	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1

	add rsp, 0x16

	pop rbp
	ret


interpret_in :
	call interpret_load_operands

	in r1
	storb r3, r1

	jmp interpret_loop_2


interpret_lod :
	call interpret_load_operands

	; r3 = valud of operand 0

	loadb r4, r5     ; r4 = [operand 1]

	mov r5, rbp
	sub r5, 0x10
	add r5, r5, r4
	loadb r5, r5 ; r5 = [[operand 1]]

	storb r3, r5

	jmp interpret_loop_2