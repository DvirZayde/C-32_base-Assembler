#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
/*
writefile.c makes the requested result files: ent. and ob. and also translate integer type code words to 32 bit computer words
*/

/*
Making the ob. file from code_array
@param	name	name of file to make the object file from
@param	code_array
@param	cc	command counter
@param	dc	data counter
@resolt	object file with the 32 bit word in the format is created
*/
void write_object(char *name, int *code_array, int cc, int dc)
{
	FILE *fd2;
	int i;
	char add[3], code[3], *fname;
	fname=strcombine(name, ".ob");
	fd2=fopen(fname, "w");
	fprintf(fd2,"%s %s\n", convert32(cc,add), convert32(MAX_RAM-dc-1,code));
	for (i=0;i<cc;i++)
		fprintf(fd2,"%s %s\n",convert32(i+START_POINT,add), convert32(code_array[i],code));
	for (i=MAX_RAM-1;i>dc;i--)
		fprintf(fd2,"%s %s\n",convert32(MAX_RAM-i-1+cc+START_POINT,add), convert32(code_array[i],code));
	fclose(fd2);
	free(fname);
}
/*
Making the ent. file from label list
@param	name	name of file to make the entry file from
@param	h	head of label list
@resolt	entry file in the format is created (only) if entry labels exist
*/
void write_entry(char *name, l_ptr h)
{
	l_ptr p;
	FILE *fd3;
	bool test;
	char sign[3], *fname;
	fname=strcombine(name, ".ent");
	test=FALSE;
	p=h;
	while (p!=NULL){
		if(p->is_entry){
			fd3=fopen(fname, "w");
			test=TRUE;
			break;
		}
		p=p->next;
	}	
	while (p!=NULL){
		if(p->is_entry)
			fprintf(fd3, "%s	%s\n", p->name, convert32(p->address, sign));
		p=p->next;
	}
	if (test)
		fclose(fd3);
	free(fname);
}
/*
convert a number in range of 0-31 to the 32 bit symbol
@param	num	number in range to convert
@return	character that match the number in 32 bit symbol
note: use only by "conver32" function
*/ 
static char bit32(int num)
{
	switch (num){case 0:return '!';case 1:return '@';case 2:return '#';case 3:return '$';case 4:return '%';case 5:return '^';case 	6:return '&';case 7:return '*';case 8:return '<';case 9:return '>';case 10:return 'a';case 11:return 'b';case 12:return 		'c';case 13:return 'd';case 14:return 'e';case 15:return 'f';case 16:return 'g';case 17:return 'h';case 18:return 'i';case 		19:return 'j';case 20:return 'k';case 21:return 'l';case 22:return 'm';case 23:return 'n';case 24:return 'o';case 25:return 		'p';case 26:return 'q';case 27:return 'r';case 28:return 's';case 29:return 't';case 30:return 'u';case 31:return 'v';}
	return '!';
}
/*
convert an int number in range of 0-(32^2-1)
@param	num	number in range to conver
@param	sign	a pointer to string to use for saving the 32bit number as string
@return	pointer to string contains the 32 bit number
*/
char* convert32(int num, char *sign)
{
	unsigned int first, second;
	first=(~(-1<<5))&num;
	second=(~(-1<<10))&num;
	second=(second>>5);
	sign[0]=bit32(second);
	sign[1]=bit32(first);
	sign[2]='\0';
	return sign;
}
