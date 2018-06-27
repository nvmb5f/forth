; drop ( a -- )
native "drop", drop
	add rsp, 8
	jmp next

;	swap ( a b -- b a )
native "swap", swap
	mov rax, [rsp]
	mov rcx, [rsp+8]
	mov [rsp+8], rax
	mov [rsp], rcx
	jmp next

; dup ( a -- a a )
native "dup", dup
	pop rax
	push rax
	push rax
	jmp next

; rot ( a b c -- b c a )
native "rot", rot
	pop rax
	pop rbx
	pop rcx
	push rbx
	push rax
	push rcx
	jmp next


; Arithmetic:

; ( x y -- [ x + y ] )
native "+", plus
	pop rax
	add [rsp], rax
	jmp next

; ( y x -- [ x * y ] )
native "*", mul
	pop rax
	pop rcx
	imul rcx
	push rax
	jmp next

; / ( x y -- [ x / y ] )
native "/", div
	pop rcx
	pop rax
	cqo
	idiv rcx
	push rax
	jmp next

; % ( x y -- [ x mod y ] )
native "%", mod
	pop rcx
	pop rax
	cqo
	idiv rcx
	push rdx
	jmp next

; - ( x y -- [x - y] )
native "-", minus
	pop rax
	sub [rsp], rax
	jmp next

; < ( x y -- [x < y] )
native "<", less
	pop rax ; y
	pop rcx ; x
	cmp rax, rcx
	jg .true
	push 0
	jmp .end
	.true:
		push 1
	.end:
		jmp next


; Logic:

; not ( a -- a' ) a' = 0 if a != 0 a' = 1 if a == 0
native "not", not
	pop rax
	test rax, rax
	jnz .zero
	push 1
	jmp .end
	.zero:
		push 0
	.end:
		jmp next

; = ( a b -- c ) c = 1 if a == b c = 0 if a != b
native "=", equals
	pop rax
	pop rcx
	cmp rax, rcx
	je .e
	push 0
	jmp .end
	.e:
		push 1
	.end:
		jmp next

; land ( a b -- a && b ) Logical and
native "land",land
	pop rax
	pop rcx
	test rax, rax
	je .zero
	push rcx
	jmp .end
	.zero:
		push rax
	.end:
		jmp next

; lor ( a b -- a || b ) Logical or
native "lor", lor
	pop rax
	pop r12
	test rax, rax
	jne .notzero
	test r12, r12
	jne .notzero
	push 0
	jmp .end
	.notzero:
		push 1
	.end:
		jmp next

; Bitwise

; and ( a b -- a & b ) Bitwise and
native "and", and
    pop rax
    and [rsp], rax
    jmp next

; or ( a b -- a | b ) Bitwise or
native "or", or
    pop rax
    or [rsp], rax
    jmp next



; ' Read word, find its XT, place on stack (or zero if no such word).
colon "'", tick, 1
	dq xt_inbuf, xt_word, xt_drop ; write to buffer
	dq xt_inbuf, xt_find ; dict search
	dq xt_dup ; word addr or 0
    branch0 .no_such_word
	dq xt_cfa ; xt
    dq xt_state, xt_fetch
    branch0 .interpret
		; state 1 - compilation mode
    dq xt_lit, xt_lit
    dq xt_comma, xt_comma
		.interpret:
			dq xt_exit

    .no_such_word:
    	dq xt_inbuf, xt_prints
    	dq xt_lit, noword_msg, xt_prints
    	dq xt_exit

; count ( str -- len ) Accepts a null-terminated string, calculates its length.
native "count", count
	pop rdi
	call string_length
	push rax
	jmp next

; printc ( str cnt -- ) Prints a certain amount of characters from string.
native "printc", printc
	mov rax, 1
	mov rdi, 1
	pop rdx
	pop rsi
	syscall
	jmp next

; . Drops element from stack and sends it to stdout.
native ".", dot
    pop rdi
    call print_int
    jmp next

; .S Shows stack contents. Does not pop elements.
native ".S", show_stack
    mov rcx, rsp
    .loop:
        cmp rcx, [stack_base]
        jae next
        mov rdi, [rcx]
        push rcx
        call print_int
        call print_newline
        pop rcx
        add rcx, 8
        jmp .loop


; init Stores the data stack base. It is useful for .S.
native "init", init
	; Initialize registers
	mov qword [state], 0
	mov rstack, rstack_start
	mov pc, main_stub
	; Initialize stack
	cmp qword [stack_base], 0
	je .start
	mov rsp, [stack_base]
	jmp .cont
	.start:
		mov [stack_base], rsp
	.cont:
		jmp next


; docol This is the implementation of any colon-word.
; The XT itself is not used, but the implementation (i_docol) is.
native "docol", docol
	sub rstack, 8
	mov [rstack], pc
	add w, 8	; предполагая размер ячейки 8 байт
	mov pc, w
	jmp next

; exit Exit from colon word.
native "exit", exit
	mov pc, [rstack]
	add rstack, 8
	jmp next

; r> Push from return stack into data stack.
native '>r', to_r
  pop rax
  rpush rax
  jmp next

; >r Pop from data stack into return stack.
native 'r>', from_r
  rpop rax
  push rax
  jmp next

; r@ Non-destructive copy from the top of return stack to the top of data stack.
native 'r@', r_fetch
	push qword[rstack]
	jmp next

; find ( str -- header_addr )
; Accepts a pointer to a string, returns pointer to the word header in dictionary.
native "find", find
	mov rsi, [last_word]
	.loop:
		mov rdi, [rsp]
		push rsi
		add rsi, 9
		call string_equals
		pop rsi
		test rax, rax
		jnz .entry
		mov rsi, [rsi]
		test rsi, rsi
		jnz .loop
	; no such word
	mov qword [rsp], 0
	jmp .end

	.entry:
		mov [rsp], rsi
	.end:
		jmp next

; cfa ( word_addr -- xt )
; Converts word header start address to the execution token
native "cfa", cfa
  pop rsi
  add rsi, 9
  .loop:
	  mov al, [rsi]
	  test al, al
	  jz .end
	  inc rsi
	  jmp .loop
  .end:
		add rsi, 2
	  push rsi
	  jmp next

; emit ( c -- ) Outputs a single character to stdout
native "emit", emit
  pop rdi
  call print_char
  jmp next

; word ( addr -- len )
; Reads word from stdin and stores it starting at address addr.
; Word length is pushed into stack
native "word", word
	pop rdi
	call read_word
	push rdx
	jmp next

; number ( str -- num len ) Parses an integer from string.
native "number", number
  pop rdi
  call parse_int ; rax: number, rdx : length
	push rax
	push rdx
  jmp next

; prints ( addr -- ) Prints a null-terminated string.
native "prints", prints
	pop rdi
	call print_string
	jmp next

; bye Exits Forthress
native "bye", bye
  mov rax, 60
  xor rdi, rdi
  syscall

; syscall ( call_num a1 a2 a3 a4 a5 a6 -- new_rax new_rdx) Executes syscall
; The following registers store arguments (according to ABI)
; rdi , rsi , rdx , r10 , r8 and r9
native "syscall", syscall
	pop r9
	pop r8
	pop r10
	pop rdx
	pop rsi
	pop rdi
	pop rax
	syscall
	push rax
	push rdx
	jmp next

; branch Jump to a location. Location is absolute.
; branch is a compile-only word.
native "branch", branch
    mov pc, [pc]
    jmp next

; branch0 Jump to a location if TOS = 0. Location is calculated in a similar way.
; branch0 is a compile-only word.
native "branch0", branch0
    pop rax
    test rax, rax
    jz .true
  	add pc, 8
		jmp .end
    .true:
		  mov pc, [pc]
		.end:
    	jmp next

; lit Pushes a value immediately following this XT.
native "lit", lit
    push qword[pc]
    add pc, 8
    jmp next

; inbuf Address of the input buffer (is used by interpreter/compiler).
const inbuf, input_buf
; mem Address of user memory.
const mem, user_mem
; last_word Header of last word address
const last_word, last_word
; state, state State cell address.
; The state cell stores either 1 (compilation mode) or 0 (interpretation mode).
const state, state
; here Points to the last cell of the word currently being defined .
const here, [here]

; execute ( xt -- ) Execute word with this execution token on TOS.
native "execute", execute
	pop w
	jmp [w]

; @ ( addr -- value ) Fetch value from memory.
native "@", fetch
	pop rax
	push qword[rax]
	jmp next

; ! ( val addr -- ) Store value by address.
native "!", write
    pop rax
    pop rdx
    mov [rax], rdx
    jmp next

; c! ( char addr -- ) Store one byte by address.
native "c!", write_char
	pop rax
	pop rdx
	mov [rax], dl
	jmp next

; c@ ( addr -- char ) Read one byte starting at addr.
native "c@", fetch_char
	pop rdi
	call read_char
	push rax
	jmp next

; , ( x -- ) Add x to the word being defined.
native ",", comma
	pop rax
	mov rsi, [here]
	mov [rsi], rax
	add qword[here], 8
	jmp next

; c, ( c -- ) Add a single byte to the word being defined.
native "c,", char_comma
	pop rax
	mov rsi, [here]
	mov [rsi], al
  add qword[here], 1
	jmp next

; create ( flags name -- )
; Create an entry in the dictionary name is the new name.
; Only immediate flag is implemented ATM.
native "create", create
	; link
	mov rcx, [last_word]
	mov rsi, [here]
	mov [rsi], rcx
	mov [last_word], rsi
	add rsi, 8
	mov byte [rsi], 0
	inc rsi
	; name
	pop rdi
	push rsi
	call string_copy
	pop rsi
	push rsi
	mov rdi, rsi
	call string_length
	pop rsi
	add rsi, rax
	inc rsi
	; flags
	pop rax
	mov [rsi], al
	inc rsi

	mov [here], rsi
	jmp next

; : Read word from current input stream and start defining it.
colon ":", colon
    .restart:
    dq xt_inbuf, xt_word
    branch0 .restart
    dq xt_lit, 0, xt_inbuf, xt_create
    dq xt_lit, 1, xt_state, xt_write
    dq xt_lit, i_docol, xt_comma
    dq xt_exit

; ; End the current word definition
colon ";", semicolon, 1
    dq xt_lit, 0, xt_state, xt_write
    dq xt_lit, xt_exit, xt_comma
    dq xt_exit
