mov rbp, rsp
call heap_init
jmp main

str_welcome : "Player Organization System\n"
str_prompt  : "> "

str_command_delete : "delete"
str_command_first  : "first"
str_command_help   : "help"
str_command_new    : "new"
str_command_next   : "next"
str_command_print  : "print"
str_command_quit   : "quit"
str_command_rename : "rename"

str_null_next : "next player was null\n"

str_first : "next player reset to first player\n"

str_enter_name : "enter name: "
str_print_name : "player name: "
str_newline    : "\n"

str_bad_command : "bad command. try help\n"

str_success : "success"
str_failure : "failure"

str_goodbye : "Goodbye!\n"

str_help_line_0 : "Available Commands\n"
str_help_line_1 : "delete first help new next print quit\n"


main :
    mov r0, str_welcome
    push r0
    call puts
    add rsp, 2

    mov r1, 0 ; r1 = pointer to first player
    mov r2, 0 ; r2 = pointer to next  player

main_loop :
    call get_command
    cmp r0, 1
    je main_loop_command_delete
    cmp r0, 2
    je main_loop_command_first
    cmp r0, 3
    je main_loop_command_help
    cmp r0, 4
    je main_loop_command_new
    cmp r0, 5
    je main_loop_command_next
    cmp r0, 6
    je main_loop_command_print
    cmp r0, 7
    je main_loop_command_quit
    cmp r0, 8
    je main_loop_command_rename
    jmp main_loop

main_loop_command_delete :
    cmp r2, 0
    je main_loop_null_next

    push r2
    call player_deconstructor
    mov r2, r1
    jmp main_loop

main_loop_command_first :
    push str_first
    call puts
    add rsp, 2
    mov r2, r1
    jmp main_loop

main_loop_command_help :
    push str_help_line_0
    call puts
    push str_help_line_1
    call puts
    add rsp, 4
    jmp main_loop

main_loop_command_new :
    call player_constructor ; allocate new player
    
    cmp r0, 0 ; if we failed to create a player, just quit
    je main_loop_command_quit

    mov r3, r0              ; r3 = new player

    push r3
    push r1
    call player_append      ; add new player to list of players
    add  rsp, 4
    mov  r1, r0

    mov r2, r1              ; reset next player to first player

    push str_enter_name     ; prompt for player name
    call puts
    add rsp, 2

    sub rsp, 0x20
    mov r4, rsp             ; r4 = player name
    push 0x20
    push r4
    call getns              ; get player name
    add rsp, 4

    push r4
    push r3
    call player_vtable_set_name ; set player name
    add rsp, 0x24

    jmp main_loop_command_first

main_loop_command_next :
    cmp r2, 0
    je main_loop_null_next

    push r2
    call player_get_next
    add rsp, 2
    mov r2, r0
    jmp main_loop

main_loop_command_print :
    cmp r2, 0
    je main_loop_null_next

    push str_print_name
    call puts
    add rsp, 2

    push r2
    call player_vtable_get_name
    push r0
    call puts
    add rsp, 4

    push str_newline
    call puts
    add rsp, 4

    jmp main_loop

main_loop_null_next :
    push str_null_next
    call puts
    add rsp, 2
    jmp main_loop_command_first

main_loop_command_quit :
    push str_goodbye
    call puts
    hlt

main_loop_command_rename :
    cmp r2, 0
    je main_loop_null_next

    push str_enter_name     ; prompt for player name
    call puts
    add rsp, 2

    sub rsp, 0x20
    mov r4, rsp             ; r4 = player name
    push 0x20
    push r4
    call getns              ; get player name
    add rsp, 4

    push r4
    push r2
    call player_vtable_set_name ; set player name
    add rsp, 0x24

    jmp main_loop



; returns 1 for delete
;         2 for first
;         3 for help
;         4 for new
;         5 for next
;         6 for print
;         7 for quit
;         8 for rename
get_command :
    push rbp
    mov rbp, rsp

    push r1
    
    ; 40 byte buffer for reading in string
    sub rsp, 0x28 ; == rbp-0x2a

get_command_loop :
    push str_prompt
    call puts
    add rsp, 2

    mov r1, rbp
    sub r1, 0x2a
    push 40
    push r1
    call getns
    add rsp, 4

    push r1
    push str_command_delete
    call strcmp
    add rsp, 4
    cmp r0, 0
    je get_command_return_delete

    push r1
    push str_command_first
    call strcmp
    add rsp, 4
    cmp r0, 0
    je get_command_return_first

    push r1
    push str_command_help
    call strcmp
    add rsp, 4
    cmp r0, 0
    je get_command_return_help

    push r1
    push str_command_new
    call strcmp
    add rsp, 4
    cmp r0, 0
    je get_command_return_new

    push r1
    push str_command_next
    call strcmp
    add rsp, 4
    cmp r0, 0
    je get_command_return_next

    push r1
    push str_command_print
    call strcmp
    add rsp, 4
    cmp r0, 0
    je get_command_return_print

    push r1
    push str_command_quit
    call strcmp
    add rsp, 4
    cmp r0, 0
    je get_command_return_quit

    push r1
    push str_command_rename
    call strcmp
    add rsp, 4
    cmp r0, 0
    je get_command_return_rename

    push str_bad_command
    call puts
    add rsp, 2
    jmp get_command_loop

get_command_return_delete :
    mov r0, 1
    jmp get_command_return

get_command_return_first :
    mov r0, 2
    jmp get_command_return

get_command_return_help :
    mov r0, 3
    jmp get_command_return

get_command_return_new :
    mov r0, 4
    jmp get_command_return

get_command_return_next :
    mov r0, 5
    jmp get_command_return

get_command_return_print :
    mov r0, 6
    jmp get_command_return

get_command_return_quit :
    mov r0, 7
    jmp get_command_return

get_command_return_rename :
    mov r0, 8
    jmp get_command_return

get_command_return :
    add rsp, 0x28
    pop r1
    pop rbp
    ret
