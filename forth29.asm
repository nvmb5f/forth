global _start

%include 'util.inc'
%include 'macro.inc'
%include 'words29.inc'

    section .bss
resq 1023
rstack_start: resq 1   ; return stack start
input_buf: resb 1024   ; global buffer
user_mem: resq 65536   ; global data for user
user_dict: resq 65536
state: resq 1          ; 0 - interpretator mode (default)
                       ; 1 - compilation mode (changes to 1 if compiling)

    section .data
last_word: dq _lw      ; a pointer to the last word in dictionary
here: dq user_dict     ; current position in memory
stack_base: dq 0

    section .rodata
noword_msg: db "> no such word", 10, 0

    section .text

_start:
  jmp i_init ; The program starts execution from the init word from word29.inc

; Program
main_stub:

  .loop:
    dq xt_inbuf, xt_word, xt_drop ; write word to buf, drop word length
    dq xt_inbuf, xt_find          ; look for it in dictionary
    dq xt_state, xt_fetch
    branch0 .interpretator_mode

    ; state 1 - compilation mode
    dq xt_swap, xt_drop
    dq xt_comma
    branch .loop

    ; state 0 - compilation mode
    .interpretator_mode:
        dq xt_dup
        branch0 .check_for_number
      dq xt_cfa
      dq xt_execute
      branch .loop

    .check_for_number:
      dq xt_drop
      dq xt_inbuf
      dq xt_number  ; (number length)
      branch0 .no_such_word
      branch .loop

    .no_such_word:
      dq xt_drop
      dq xt_lit, noword_msg, xt_prints
      branch .loop

next:
  mov w, pc
  add pc, 8
  mov w, [w]
  jmp [w]
  
