' drop g"
( a -- )
Drop the topmost element of the stack.
" doc-word

' swap g" 
( a b -- b a )
Swap two topmost elements of the stack.
" doc-word

' dup g" 
( a - a a )
Duplicate the cell on top of the stack.
" doc-word


' rot g"
( a b c -- b c a )
Rotate three topmost elements of the stack. The 3rd element goes to TOS.
" doc-word


( Arithmetic )

' + g"
( x y -- [ x + y ] )
Add two topmost elements of the stack and place the result on TOS.
" doc-word

' * g"
( y x -- [ x * y ] )
Multiply two topmost elements of the stack and place the result on TOS.
" doc-word
	
' / g"
( x y -- [ x / y ] )
Divide the 2nd element of the stack by the 1st one and place the quotient on TOS.
" doc-word
	

' % g"
( x y -- [ x mod y ] )
Divide the 2nd element of the stack by the 1st one and place the remainder on TOS.
" doc-word

' - g"
( x y -- [x - y] )
Subtract the 2nd element of the stack from the 1st one and place the result on TOS.
" doc-word

' < g"
( x y -- [x < y] )
Compare two topmost elements of the stack and place the result on TOS.
The result is 1 if the 2nd element is less than the 1st one. Otherwise, the result is 0. 
" doc-word


( Logic )

' not "g
( a -- a' )
Invert the topmost element of the stack. If it's 0, replace TOS with 1, otherwise with 0.
" doc-word

' = g"
( a b -- c )
Compare two topmost elements of the stack and place the result on TOS.
The result is 1 if elements are equal. Otherwise, the result is 0. 
" doc-word

' land g"
( a b -- a && b ) 
Place the result of logical AND operation on two topmost elements of the stack on TOS.
The result is 0 if at least one of the elements is 0.
" doc-word 

' lor g"
( a b -- a || b ) 
Place the result of logical OR operation on two topmost elements of the stack on TOS.
The result is 0 if both of the elements is 0.
" doc-word 


( Bitwise )

' and g"
( a b -- a & b )
Place the result of bitwise AND operation on two topmost elements of the stack on TOS.
" doc-word 
  
' or g"
( a b -- a | b )
Place the result of bitwise OR operation on two topmost elements of the stack on TOS.
" doc-word 

' ' g"
Read word, find its XT and place on stack (place zero if no such word).
" doc-word

' count g"
( str -- len ) 
Calculate length of a null-terminated string.
" doc-word

' printc g"
( str cnt -- ) 
Print a certain amount of characters from string.
" doc-word

' . g"
Drop element from stack and send it to stdout.
" doc-word

' or g"
; .S Shows stack contents. Does not pop elements.
native ".S", show_stack
" doc-word

' init g"
Store the data stack base. 
" doc-word

' docol g"
This is the implementation of any colon-word.
" doc-word

' exit g"
Exit from colon word.
" doc-word

' r> g"
Push from return stack into data stack.
" doc-word

' >r g"
Pop from data stack into return stack.
" doc-word

' r@ g"
Non-destructive copy from the top of return stack to the top of data stack.
" doc-word

' find g"
( str_ptr -- header_addr )
Get pointer to the word header in dictionary.
" doc-word

' cfa g"
( word_addr -- xt )
Convert word header start address to the execution token.
" doc-word

' emit g"
( c -- ) 
Output a single character to stdout.
" doc-word

' word g"
( addr -- len )
Read word from stdin and store it starting at address addr.
Word length is pushed into stack.
" doc-word

' number g"
( str -- num len ) 
Parse an integer from string.
" doc-word

' prints g"
( addr -- ) 
Print a null-terminated string.
" doc-word

' bye g"
Exit Forthress.
" doc-word

' syscall g"
( call_num a1 a2 a3 a4 a5 a6 -- new_rax new_rdx)
Execute syscall. According to ABI, the following registers store arguments:
rdi , rsi , rdx , r10 , r8 and r9.
" doc-word

' branch g"
Jump to a location. Location is absolute.
Branch is a compile-only word.
" doc-word

' branch0 g"
Jump to a location if topmost element is 0. Location is absolute.
Branch0 is a compile-only word.
" doc-word

' lit g"
Pushe a value immediately following this XT.
" doc-word

' execute g"
( xt -- ) 
Execute word with this execution token on TOS.
" doc-word

' lit g"
; @ ( addr -- value ) Fetch value from memory.
" doc-word

' ! g"
( val addr -- ) 
Store value by address.
" doc-word

' c! g"
( char addr -- ) 
Store one byte by address.
" doc-word

' c@ g"
( addr -- char )
Read one byte starting at addr.
" doc-word

' , g"
( x -- )
Add x to the word being defined.
" doc-word

' c, g"
( c -- ) 
Add a single byte to the word being defined.
" doc-word

' create g"
( flags name -- )
Create an entry in the dictionary with the given name.
Only immediate flag is implemented ATM.
" doc-word

' : g"
Read word from current input stream and start defining it.
" doc-word

' ; g"
End the current word definition
" doc-word

