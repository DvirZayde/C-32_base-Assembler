assembler:	     assembler.o pre_assembler.o macro.o helper.o label.o f_pass.o s_pass.o instruct.o command.o writefile.o assembler.h
		          gcc -g -ansi -Wall -pedantic assembler.o  pre_assembler.o macro.o helper.o label.o f_pass.o s_pass.o instruct.o command.o writefile.o -o assembler
assembler.o:	     assembler.c
		          gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
pre_assembler.o:	pre_assembler.c
		          gcc -c -ansi -Wall -pedantic pre_assembler.c -o pre_assembler.o
macro.o:	          macro.c
		          gcc -c -ansi -Wall -pedantic macro.c -o macro.o
helper.o:	          helper.c
		          gcc -c -ansi -Wall -pedantic helper.c -o helper.o
label.o:	          label.c
		          gcc -c -ansi -Wall -pedantic label.c -o label.o
f_pass.o:	          f_pass.c
		          gcc -c -ansi -Wall -pedantic f_pass.c -o f_pass.o
s_pass.o:	          s_pass.c
		          gcc -c -ansi -Wall -pedantic s_pass.c -o s_pass.o
instruct.o:	     instruct.c
		          gcc -c -ansi -Wall -pedantic instruct.c -o instruct.o
command.o:	     command.c
		          gcc -c -ansi -Wall -pedantic command.c -o command.o
writefile.o:	     writefile.c
		          gcc -c -ansi -Wall -pedantic writefile.c -o writefile.o
