r6: mov S1.1 ,LEN
add r2,STR
LO#OP: jmk END
macro m1
inc K
mov S1.2 , ,r3
endmacro
prn #-5 r5
sub r1 r4
m1
bne LOOP
END: hltg
STR: .string abc def"
LEN: .data 6 , -9,  15, 33, 750    	
K: .data 22
S1: .struct +7, "cd
.extern K
