; implements a simple heap
; the heap structure looks like this
; struct _heap {
;   bool   this_bin_allocated : 1;
;   bool   next_bin_allocated : 1;
;   uint16 this_bin_size      : 14;
; };
;
; an empty heap will show:
;   memory[heap_base] = heap_size
; a heap with one 8-byte allocated object may show:
;   memory[heap_base] = 0xC008
;   // 8 bytes of user memory
;   memory[heap_base + 0x12] = heap_base - 0x12


heap_base : 0x2000
heap_size : 0x3ffc

heap_init :
    push r1
    mov  r0, heap_size
    load r0, heap_size
    mov  r1, heap_base
    load r1, r1
    stor r1, r0
    pop  r1
    ret



malloc :
    push rbp
    mov rbp, rsp
    push r1
    push r2
    push r3
    push r6
    push r7 ; r7 = number of bytes to allocate

    mov  r7, rbp
    add  r7, 4
    load r7, r7

    mov r0, heap_base ; r0 = pointer to next heap struct
    load r0, r0

; find a heap block big enough to hold object
malloc_loop :
    ; are we out of memory
    ; search for unallocated heap block
    load r1, r0 ; r1 = heap struct value
    mov  r2, r1
    and  r2, 0x8000
    cmp  r2, 0x8000
    jne  malloc_unallocated_bin

; advance to next heap block
malloc_loop_next :
    ; unallocated block not found, move to next block
    mov r2, r1
    and r2, 0x3fff
    add r0, r0, r2
    add r0, 2
    ; make sure r0 is not beyond heap_base + heap_size
    mov  r2, heap_base
    load r2, r2
    mov  r1, heap_size
    load r1, r1
    add  r2, r2, r1
    cmp  r0, r2
    jge  malloc_return_null
    jmp  malloc_loop

; check an unallocated bin to see if we will fit
malloc_unallocated_bin :
    ; is this bin large enough
    mov r2, r1
    and r2, 0x3fff
    cmp r7, r2
    jle malloc_allocate
    jmp malloc_loop_next

; allocate memory in this bin
malloc_allocate :
    ; if we can split this bin, split it
    mov r2, r1
    and r2, 0x3fff ; r2 = size of this bin
    mov r6, r7
    add r6, 6      ; r6 = needed bytes + 6
    cmp r6, r2
    jl malloc_allocate_split
malloc_allocate_after_split :
    ; set this bin as allocated
    or   r1, 0x8000
    stor r0, r1
    ; return a pointer after this malloc struct
    add r0, 2
    jmp malloc_return

malloc_allocate_split :
    mov r2, r1     ; r2 = next bin allocated bit
    and r2, 0xc000

    mov r3, r1
    and r3, 0x3fff
    sub r3, r3, r7 
    sub r1, r1, r3 
    or  r1, 0x4000 ; r1 = finalized current bin struct
    sub r3, 2      ; r3 = size of new bin
    or  r2, r2, r3 ; r2 = finalized next bin struct
    add r3, r0, r7
    add r3, 2      ; r3 = location of next bin

    stor r3, r2
    jmp malloc_allocate_after_split

malloc_return_null :
    mov r0, 0
    jmp malloc_return

malloc_return :
    pop r7
    pop r6
    pop r3
    pop r2
    pop r1
    pop rbp
    ret



free :
    push rbp
    mov rbp, rsp

    push r1
    push r2
    push r3
    push r4

    mov r0, rbp
    add r0, 4
    load r0, r0
    sub r0, 2 ; r0 = address of heap struct to free

    load r1, r0 ; r1 = value of heap struct to free

    ; check if there is a heap struct following this one
    mov r2, r1
    and r2, 0x4000
    cmp r2, 0x4000
    je free_next_exists
free_after_next_exists :
    ; remove allocated bit from struct
    and r1, 0x7fff
    stor r0, r1

    pop r4
    pop r3
    pop r2
    pop r1
    pop rbp
    ret

free_next_exists :
    ; get the next struct, put it in r2
    mov r2, r1
    and r2, 0x3fff

    add  r2, r2, r0
    add  r2, 2  ; r2 = location of next struct
    load r3, r2 ; r3 = value of next struct
    mov r4, r3
    and r4, 0x8000
    cmp r4, 0x8000
    ; if the next struct is allocated do nothing
    je free_after_next_exists
    ; if the next struct is not allocated, combine these two structs
    mov r4, r3
    and r4, 0x4000 ; r4 = next struct has following struct
    and r3, 0x3fff ; r3 = size of next struct

    and r1, 0xbfff
    or  r1, r1, r4 ; r1 takes next struct bit from r4
    add r1, r1, r3
    add r1, 2
    jmp free_after_next_exists
