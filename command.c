#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "assembler.h"
/*
command.c is hundeling lines after founded as command lines and generate the code for first and second pass functions
*/
extern cmd op_cmd[]; 

static bool get_2op_cmd(char *line, int i, int type, int *code_array, int *cc);
static bool get_1op_cmd(char *line, int i, int type, int *code_array, int *cc);
static bool get_no_op_cmd(char *line, int i, int type, int *code_array, int *cc);
static void write_cmd(int type, int sou, int des, int *code_array, int *cc);
static void put_op_value(char *op_s, char *op_d, int sou, int des, int *code_array, int *cc);
static void put_1op_value(char *op, int type, int *code_array, int *cc, int reg);
static int get_add_type(char *add);
static bool put_address(FILE *fd2, l_ptr h, char* label, int *code_array, int *c);

/*
gets command line and send it to the suited function according to the amount of operands
@param	line	command line
@param	i	pointer to index in the line
@param	type	the type of command
@param	code_array
@param	cc	command counter
@result	insert the commands code word for first pass
@return TRUE if all okey FALSE if problem found
*/
bool insert_cmd(char *line, int i, int type, int *code_array, int *cc)
{
	bool test;
	switch(type){
		case OP_MOV: 
		case OP_CMP: 
		case OP_ADD: 
		case OP_SUB:
		case OP_LEA:test=get_2op_cmd(line, i, type, code_array, cc);break;
		case OP_NOT:
		case OP_CLR:
		case OP_INC:
		case OP_DEC:
		case OP_JMP:
		case OP_BNE:
		case OP_GET:
		case OP_PRN:
		case OP_JSR:test=get_1op_cmd(line, i, type, code_array, cc);break;
		case OP_RTS:
		case OP_HLT:test=get_no_op_cmd(line, i, type, code_array, cc);break;
	}
	return test;
}
/* 
hundle the 2 operands commands
@param	line	command line
@param	i	pointer to index in the line
@param	type	the type of command
@param	code_array
@param	cc	command counter
@result	insert the command line code words
@return TRUE if all okey FALSE if problem found
*/
static bool get_2op_cmd(char *line, int i, int type, int *code_array, int *cc)
{
	int sou, des, count_comma;
	char *op1, *op2;
	bool test;
	count_comma=0;
	test=TRUE;
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if((sou=get_add_type(op1=read_word2(line, i)))==-1){
		printf("ERROR: un-known or faulty first operand/address type: %s\n", op1);
		test=FALSE;	
	}
	i+=strlen(op1);
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if (line[i++]!=','){
		printf("ERROR: comma is missing between command's operands\n");
		test=FALSE;
		i--;
	}
	for(;line[i]=='\t' || line[i]==' ' || line[i]==',';i++)/*skip white and counts comma*/
		if(line[i]==',')
			count_comma++;
	if (count_comma>0){
		printf("ERROR: too many commas between operands\n");
		test=FALSE;
	}
	if((des=get_add_type(op2=read_word2(line, i)))==-1){
		printf("ERROR: un-known or faulty second operand/address type: %s\n", op2);
		test=FALSE;	
	}
	i+=strlen(op2);
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if(line[i]!='\n' && line[i]!='\0' && line[i]!=EOF){
		printf("ERROR: extra text in the end of command\n");
		test=FALSE;	
	}
	if (des==IMMIDIATE && type!=OP_CMP){ /*checks address type fit command type*/
		printf("ERROR: immidiate second operand dont fit command type %s\n", op_cmd[type].cmd_name);
		test=FALSE;	
	}
	if ((sou==IMMIDIATE || sou==REGISTER) && type==OP_LEA){ /*checks address type fit command type*/
		printf("ERROR: immidiate or register first operand dont fit 'lea' command\n");
		test=FALSE;	
	}
	if (test){
		write_cmd(type, sou, des, code_array, cc);
		put_op_value(op1, op2, sou, des, code_array, cc);
	}
	free(op1);
	free(op2);
	return test;
}
/*
hundle the 1 operands commands
@param	line	command line
@param	i	pointer to index in the line
@param	type	the type of command
@param	code_array
@param	cc	command counter
@result	insert the command line code words
@return TRUE if all okey FALSE if problem found
*/
static bool get_1op_cmd(char *line, int i, int type, int *code_array, int *cc)
{
	int des;
	char *op1;
	bool test=TRUE;
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if((des=get_add_type(op1=read_word2(line, i)))==-1){
		printf("ERROR: un-known or faulty operand/address type: %s\n", op1);
		test=FALSE;	
	}
	i+=strlen(op1);
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if(line[i]!='\n' && line[i]!='\0' && line[i]!=EOF){
		printf("ERROR: extra text in the end of command\n");
		test=FALSE;	
	}
	if (des==IMMIDIATE && type!=OP_PRN){ /*checks address type fit command type*/
		printf("ERROR: immidiate operand don't fit command type %s\n", op_cmd[type].cmd_name);
		test=FALSE;	
	}
	if (test){
		write_cmd(type, 0, des, code_array, cc);
		put_1op_value(op1, des, code_array, cc, DREGISTER);
	}
	free(op1);
	return test;
}
/*
hundle the 0 operands commands
@param	line	command line
@param	i	pointer to index in the line
@param	type	the type of command
@param	code_array
@param	cc	command counter
@result	insert the command line code words
@return TRUE if all okey FALSE if problem found
*/
static bool get_no_op_cmd(char *line, int i, int type, int *code_array, int *cc)
{
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if(line[i]!='\n' && line[i]!='\0' && line[i]!=EOF){
		printf("ERROR: extra text in the end of command\n");
		return FALSE;	
	}
	write_cmd(type, 0, 0, code_array, cc);
	return TRUE;
}
/*
generate the first code word of a command line
@param	type	the type of command
@param	sou	surce addressing type
@param	des	destination addressing type
@param	code_array
@param	cc	command counter
@result	generate and insert the the first command code word
*/
static void write_cmd(int type, int sou, int des, int *code_array, int *cc)
{
	int code;
	code=(type<<OPCODE)+(sou<<ADDS)+(des<<ADDD);
	code_array[(*cc)++]=code;
}
/*
hundle and generate code words of 2 operands commands line
@param	op_s	source operand in string form
@param	op_d	destination operand in string form
@param	sou	surce addressing type
@param	des	destination addressing type
@param	code_array
@param	cc	command counter
@result	generate and insert the the 2 operands code words
*/
static void put_op_value(char *op_s, char *op_d, int sou, int des, int *code_array, int *cc)
{
	char val1, val2;
	if (des==REGISTER && sou==REGISTER){
		val1=(char)atoi(op_s+1);/*skips r that mark the operand*/
		val2=(char)atoi(op_d+1);
		code_array[(*cc)++]=(val1<<SREGISTER)+(val2<<DREGISTER);/*enter the two registers together*/
		return;
	}
	put_1op_value(op_s, sou, code_array, cc, SREGISTER);
	put_1op_value(op_d, des, code_array, cc, DREGISTER);
	
}
/*
generate code words of an operand
@param	op	operand in string form
@param	type	operand addressing type
@param	code_array
@param	cc	command counter
@param	reg	tells if registr will code as source or destination register
@result	generate code words for operand
*/
static void put_1op_value(char *op, int type, int *code_array, int *cc, int reg)/*the reg variant helps to code a register if found*/
{
	char val1;
	if (type==REGISTER || type==IMMIDIATE){
		val1=(char)atoi(op+1);
		if(type==REGISTER)
			code_array[(*cc)++]=(val1<<reg);
		else
			code_array[(*cc)++]=(val1<<OPERAND);
	}
	else{/*type is acces to struct or direct*/
		code_array[(*cc)++]=NO_ADDR;/*-1 is easy to track in the second pass*/
		if (type==ACCESS)/*type is access*/
			code_array[(*cc)++]=(get_access_index(op)<<ACCINDX);
	}
}
/*
find operand address type
@param	add	operand in string form
@return	 operand's address type, if invalid operand return -1
*/
static int get_add_type(char *add)
{
	int i, val;
	if (add[0]=='#'){
		if(add[1]!='+' && add[1]!='-' && !isdigit(add[1]))
			return -1;
		for(i=2; add[i]!='\0'; i++)
			if(!isdigit(add[i]))
				return -1;
		val=atoi(add+1);
		if (val<-S8BIT || val>S8BIT-1){/*checks that operand is in 8 bits signed range*/
			printf("ERROR:immidiate value in command exceed 8 bits: \"%d\"\n", val);
			return -1;
		}
		return IMMIDIATE;
	}
	if(add[0]=='r' && add[1]<='7' && add[1]>='0' && add[2]=='\0'){/*r0 - r7*/
		return REGISTER;
		/*
		if(strlen(add)!=2)
			return -1;
		val=(int)atoi(add+1);
		if(val>7 || val<0) only 0-7 register are available
			return -1;
		return REGISTER;*/
	}
	if (isalpha((int)add[0])){
		for(i=1; isalnum((int)add[i]); i++);
		if (add[i]=='\0')
			return DIRECT;
		if(add[i++]=='.'){
			if (get_access_index(add)!=-1)/*return 1, 2 or if invalid return -1*/
				return ACCESS;
		}
	}
	return -1;
}
/*
Add the missing code words for labels adress operands in the second pass
@param	fd2	pointer to external file
@param	h	label list pointer
@param	line	command line
@param	i	index in line
@param	code_array
@param	c	pointer to command_array
@result	relevant code word is complited
@return TRUE if all okey FALSE if problem found
*/
bool add_address(FILE *fd2, l_ptr h, char *line, int i, int *code_array, int *c)
{
	char *word;
	int type, j;
	bool test, reg;/*reg is a flag to fine two register operands in command line*/
	reg=FALSE;
	test=TRUE;
	(*c)++;
	for(;line[i]=='\t' || line[i]==' ';i++); /*skip white*/
	for (j=0; j<2; j++){
		word=read_word2(line, i);
		i+=strlen(word);
		if ((type=get_add_type(word))==-1){
			free(word);
			return test;
		}
		if(type==DIRECT || type==ACCESS){
			if (type==ACCESS)
				remove_index_struct(word);
			if(!put_address(fd2, h, word, code_array, c)){
				printf("ERROR:operant of address is not exist:\"%s\"\n", word);
				test=FALSE;
			}
			if (type==ACCESS)
				(*c)++ ; /*if struct skips it's index*/
		}
		else if(type==IMMIDIATE)
			(*c)++;
		else{/*REGISTER*/
			(*c)++;
			if (reg)
				(*c)--;
			reg=TRUE;
		}
		for(;line[i]=='\t' || line[i]==' ' || line[i]==',';i++);/*skip white and ','*/
		free(word);
	}
	return test;
}
/*
Generate the code word for get_adress function
@param	fd2	pointer to external file
@param	h	label list pointer
@param	line	command line
@param	i	index in line
@param	code_array
@param	c	pointer to command_array
@result code word is generated
@result if external label is used than add the word address to external file
@return TRUE if all okey FALSE if problem found
*/
static bool put_address(FILE *fd2, l_ptr h, char* label, int *code_array, int *c)
{
	l_ptr p;
	char sign[3];
	p=h;
	while (p!=NULL){
		if(!strcmp(p->name, label)){
			if (p->type==EXTERN){
				fprintf(fd2,"%s	%s\n", p->name, convert32((*c)+100,sign));
				code_array[(*c)++]=EXTERNAL;
			}
			else
				code_array[(*c)++]=((p->address)<<OPERAND) + RELOCATABLE;
			return TRUE;
		}
		p=p->next;
	}
	return FALSE;
}
