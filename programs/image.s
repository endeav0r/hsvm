main :
	mov rbp, rsp

	sub rsp, 0x1000
	mov r1, rsp ; r1 = base of encoded image
	; read in first 2 bytes, which is size and may be contain null bytes
	in r3
	storb r1, r3
	in r3
	mov r4, 1
	add r4, r4, r1
	storb r4, r3
	mov r2, 2

main_loop :
	cmp   r2, 0x1000
	je    main_done
	in    r3
	cmp   r3, 0xffff
	je    main_done
	add   r4, r1, r2
	storb r4, r3
	cmp   r3, 0x0000
	je    main_loop_done
	add   r2, 1
	jmp main_loop

main_loop_done :
	push r1
	call print_image

main_done :
	hlt


; void print_image (uint8_t * encoded_image);
print_image :
	push rbp
	mov  rbp, rsp
	
	push r1
	push r2
	push r3
	push r4

	mov  r1, rbp
	add  r1, 4
	load r1, r1 ; r1 = base of encoded image

	push r1
	call image_size
	add rsp, 2

	mov r2, r0 ; r2 = size of decoded image

	sub rsp, rsp, r2
	mov r3, rsp ; r3 = base of decoded image

	push r3
	push r1
	call decode_image
	add  rsp, 4

	mov r4, 0
print_image_loop :
	cmp   r4, r2
	je    print_image_done
	add   r1, r3, r4
	loadb r1, r1
	out   r1
	add   r4, 1
	jmp   print_image_loop



print_image_done :
	add rsp, rsp, r2

	pop r4
	pop r3
	pop r2
	pop r1
	pop rbp
	ret



; uint16_t image_size (uint8_t * encoded_image);
image_size :
	push rbp
	mov  rbp, rsp

	mov  r0, rbp
	add  r0, 4
	load r0, r0

	load r0, r0

	pop rbp
	ret

; void decode_image (uint8_t * encoded_image, uint8_t * dst);
decode_image :
	push rbp
	mov rbp, rsp

	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7

	mov  r1, rbp
	add  r1, 4
	load r1, r1 ; r1 = base of encoded image

	mov  r2, rbp
	add  r2, 6
	load r2, r2 ; r2 = base to decode img into

	push r1
	call image_size
	add rsp, 2
	mov r3, r0  ; r3 = size of encoded image

	add r1, 2
	mov r4, 0   ; r4 = index into base of encoded image - 2
	mov r5, 0   ; r5 = index into decoded image

decode_image_loop :
	cmp   r5, r3
	jge   decode_image_done ; if we decoded image is full
	add   r6, r1, r4
	loadb r6, r6 ; r6 = byte in encoded image stream
	cmp   r6, 0x0000
	je    decode_image_done ; null byte = done
	cmp   r6, 0x0080
	jge   decode_image_rle

	add   r0, r2, r5 ; r0 = address in dst
	storb r0, r6
	add   r4, 1
	add   r5, 1
	jmp decode_image_loop

decode_image_rle :
	and   r6, 0x007f ; r6 = RLE length
	add   r4, 1
	add   r7, r1, r4
	loadb r7, r7 ; r7 = RLE byte
decode_image_rle_loop :
	cmp   r6, 0x0000
	je    decode_image_rle_loop_done
	add   r0, r2, r5
	storb r0, r7
	add r5, 1
	sub r6, 1
	jmp decode_image_rle_loop

decode_image_rle_loop_done :
	add r4, 1
	jmp decode_image_loop

decode_image_done :
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop rbp
	ret