mov rbp, rsp
jmp main

key0 : 0xd686
key1 : 0x33c1
key2 : 0x96b4
key3 : 0xeddc

str_pass : "pass\n"
str_fail : "fail\n"

main :
	mov r0, key0
	mov r1, 0

loop :
	add r2, r1, r0
	loadb r2, r2
	xor r2, 0x0077
	in r3
	cmp r2, r3
	jne fail
	add r1, 1
	cmp r1, 8
	je pass
	jmp loop

pass :
	push str_pass
	call puts
	hlt

fail :
	push str_fail
	call puts
	hlt

