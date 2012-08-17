; player base vtable layout
; [0] = player_set_name
; [2] = player_get_name
;
; player vtable layout
; [0] = vptr (points to player base vtable)
; [2] = char * name
; [4] = pointer to next player


player_base_vtable   : player_set_name
player_base_vtable_1 : player_get_name



player_constructor :
    push rbp
    mov  rbp, rsp

    push r1
    push r2

    push 6
    call malloc
    add  rsp, 2

    cmp r0, 0
    je player_constructor_return

    mov r1, r0

    mov  r2, player_base_vtable
    stor r1, r2

    add  r1, 2
    mov  r2, 0
    stor r1, r2

    add  r1, 2
    stor r1, r2

player_constructor_return :
    pop r2
    pop r1
    pop rbp
    ret



player_deconstructor :
    push rbp
    mov  rbp, rsp
    push r1

    mov  r0, rbp
    add  r0, 4
    load r0, r0

    mov  r1, r0
    add  r1, 2
    load r1, r1
    cmp  r1, 0
    jne  player_deconstructor_free_name
player_deconstructor_after_free_name :

    push r0
    call free
    pop r0

    pop  r1
    pop  rbp
    ret

player_deconstructor_free_name :
    push r0
    push r1
    call free
    pop r1
    pop r0
    jmp player_deconstructor_after_free_name



player_get_next :
    push rbp
    mov rbp, rsp

    mov r0, rbp
    add r0, 4
    load r0, r0
    add r0, 4
    load r0, r0

    pop rbp
    ret



player_vtable_set_name :
    mov r0, rsp
    add r0, 2
    load r0, r0 ; r0 = object
    load r0, r0 ; r0 = base vtable
    load r0, r0 ; ro = player_set_name
    mov rip, r0


player_set_name :
    push rbp
    mov  rbp, rsp

    push r7
    push r1
    push r2

    mov  r7, rbp
    add  r7, 4
    load r7, r7 ; r7 = player object

    mov  r1, rbp
    add  r1, 6
    load r1, r1 ; r1 = string to copy

    push r1
    call strlen
    add rsp, 2

    add  r0, 1
    push r0
    call malloc
    add rsp, 2

    cmp r0, 0
    je player_set_name_return

    mov  r2, r0 ; r2 = location of copied string
    push r1
    push r0
    call strcpy
    add rsp, 4

    add  r7, 2
    stor r7, r2

player_set_name_return :
    pop r7
    pop r1
    pop r2
    pop rbp
    ret



player_vtable_get_name :
    mov r0, rsp
    add r0, 2
    load r0, r0 ; r0 = object
    load r0, r0 ; r0 = base vtable
    add r0, 2   ; r0 = base_vtable + 2
    load r0, r0 ; r0 = player_get_name
    mov rip, r0

player_get_name :
    push rbp
    mov  rbp, rsp

    mov  r0, rbp
    add  r0, 4
    load r0, r0

    add  r0, 2
    load r0, r0

    pop rbp
    ret



; returns pointer to first player
player_append :
    push rbp
    mov  rbp, rsp

    push r1
    push r2
    push r3

    mov  r0, rbp
    add  r0, 4
    load r0, r0 ; r0 = object to append to

    mov  r1, rbp
    add  r1, 6
    load r1, r1 ; r1 = object to append

    cmp r0, 0
    je player_append_null

    mov r3, r0 ; r3 = saved first object

player_append_loop :
    mov r2, r0
    add r2, 4
    load r2, r2 ; r2 = pointer to next player

    cmp r2, 0
    je player_append_next_null
    mov r0, r2
    jmp player_append_loop

player_append_next_null :
    add  r0, 4
    stor r0, r1
    mov  r0, r3
    jmp player_append_return

player_append_null :
    mov r0, r1
    jmp player_append_return

player_append_return :
    pop r3
    pop r2
    pop r1
    pop rbp
    ret