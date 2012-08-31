mov rbp, rsp
jmp main

str_welcome : "Welcome to the news system!\n"
str_prompt  : "> "

str_command_help : "help"
str_command_read : "read"
str_command_quit : "quit"

str_bad_command : "bad command. try help\n"

str_goodbye : "Goodbye!\n"

str_help_line_0 : "Available Commands\n"
str_help_line_1 : "read help quit\n"

str_da_news : "It was reported in the Pwnie Times that room standards are lacking and the absence of interior guard has led to a general lack of disorder within the corps.\n"

; rbp - 0x28 = 40 char buf for input
main :
    mov r0, str_welcome 
    push r0
    call puts
    add rsp, 2

main_loop :
    call news_command
    cmp r0, 1
    je main_loop_command_help
    cmp r0, 2
    je main_loop_command_read
    cmp r0, 3
    je main_loop_command_quit
    jmp main_loop

main_loop_command_help :
    push str_help_line_0
    call puts
    push str_help_line_1
    call puts
    add rsp, 4
    jmp main_loop

main_loop_command_quit :
    push str_goodbye
    call puts
    hlt

main_loop_command_read :
    push str_da_news
    call puts
    add rsp, 2
    jmp main_loop

; returns 1 for help
;         2 for news
;         3 for quit
news_command :
    push rbp
    mov rbp, rsp

    push r1
    
    ; 40 byte buffer for reading in string
    sub rsp, 0x28 ; == rbp-0x2a

news_command_loop :
    push str_prompt
    call puts
    add rsp, 2

    mov r1, rbp
    sub r1, 0x2a
    push r1
    call gets

    push str_command_help
    call strcmp
    add rsp, 4
    cmp r0, 0
    je news_command_return_help

    push r1
    push str_command_quit
    call strcmp
    add rsp, 4
    cmp r0, 0
    je news_command_return_quit

    push r1
    push str_command_read
    call strcmp
    add rsp, 4
    cmp r0, 0
    je news_command_return_read

    push str_bad_command
    call puts
    add rsp, 2
    jmp news_command_loop

news_command_return_help :
    mov r0, 1
    jmp news_command_return

news_command_return_quit :
    mov r0, 3 
    jmp news_command_return

news_command_return_read :
    mov r0, 2
    jmp news_command_return

news_command_return :
    add rsp, 0x28
    pop r1
    pop rbp
    ret
