macro m1
inc r2
mov A,r1
endmacro

macro Nil mov r2,r5
jmp A
.extern HOLLY
endmacro

m1

m1
A: .data 55

Nil

add HOLLY ,r0
