.entry LOOP 
macro dvir STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22 endmacro

.entry LENGTH
DVIR: .extern L3
.extern W
MAIN: mov S1.1, W
add r2,STR
LOOP: jmp W
dvir
S1: .struct 8, "ab"
prn #-5
sub r1, r4
inc K
mov S1.2, r3
bne L3
END: hlt


