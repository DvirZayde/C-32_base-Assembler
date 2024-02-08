/*This program is an assembler. It gets from the main arguments names of files, spread macros,
read the command and instruction lines and makes an object file in 32bit for the computer to use.
It also makes an entry and external files for data that is used by or from other locations.
If finds any problem in assembly text will print in std-out a proper error massage with file name and line (somtimes error line and file name is shown in the next line of error massage in std-out)
Created by Dvir Zayde
Date 10/08/2022
*/ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

static bool hundlefile(char*);
/*boolean flag that is true if problems in specific file found*/
bool isfail;
/*boolean flag that is true if the file has external labels use*/
bool is_there_ext;
/*cmd type array that helps finding commands word*/
const cmd op_cmd[]={{"mov", OP_MOV},{"cmp", OP_CMP},{"add", OP_ADD},{"sub", OP_SUB},{"not", OP_NOT},{"clr", OP_CLR},{"lea",OP_LEA},{"inc",OP_INC},{"dec",OP_DEC},{"jmp",OP_JMP},{"bne",OP_BNE},{"get",OP_GET},{"prn",OP_PRN},{"jsr",OP_JSR},{"rts",OP_RTS},{"hlt",OP_HLT}, {"",OP_NONE}};
/*instruction type array that helps finding instruction word*/
const instruction ins[]={{".data", DATA},{".string", STRING},{".struct", STRUCT},{".extern", ENTRY},{".entry", EXTERN},{"",INS_NONE}};
/*main function takes the arguments and sends them to hundlefile seperatly*/
int main (int argc, char *argv[])
{
	int i ;
	for (i=1;i < argc; i++)
	{
		hundlefile(argv[i]);
	}
	return 0;
}
/*Function get file name open it and doing the all the requierd procces, return TRUE or FALSE if found problems (returns not in use by the program for now)
@param	name of hundled file
@result	all the files that requiered
*/
static bool hundlefile(char *name)
{
	char *fname, *fname2;/*will hold file name before and after pre-assembler*/
	FILE *fd1, *fd2;
	int code_array[MAX_RAM];/*will hold the code words(data and commands) until converting to 32 bit*/
	l_ptr h; /*This is the label table structure*/
	int cc=0,dc=MAX_RAM-1; /*cmd counter and date counter that start at the end of code array*/
	isfail = FALSE; /* A flag to tell us if a fail accour reset to false for every file*/
	is_there_ext= FALSE;/*A flag to find if there are external label in file*/
	h = NULL; /*This is the label table structure*/
	fname=strcombine(name, ".as");
	fd1=fopen(fname, "r");
	if (fd1 == NULL) {
		printf("ERROR: file \"%s\" has not been found or unable to open\n", fname);
		free(fname);
		return FALSE;
	}
	if(!(fname2=pre_assembler(fd1, name))){
		fclose(fd1);
		free(fname);
		return FALSE;
	}
	fclose(fd1);
	free(fname);
	fd1=fopen(fname2, "r");
	if (fd1 == NULL) {
		printf("ERROR: file \"%s\" has not been found or unable to open\n", fname);
		free(fname);
		return FALSE;
	}
	if(!f_pass(fd1, fname2, code_array, &cc, &dc, &h)){
		fclose(fd1);
		free(fname2);
		return FALSE;
	}
	rewind(fd1);
	if (is_there_ext){
		fname=strcombine(name, ".ext");/*will be use if external exsist and delet if not*/
		fd2=fopen(fname, "w");
		free(fname);
	}
	if(!s_pass(fd1, fd2, fname2, code_array, cc, &h)){
		fclose(fd1);
		free(fname2);
		return FALSE;
	}
	write_object(name,code_array, cc, dc);
	write_entry(name, h);
	fclose(fd1);
	if (is_there_ext)
		fclose(fd2);
	free(fname2);
	free_labels(&h);
	return TRUE;
}
