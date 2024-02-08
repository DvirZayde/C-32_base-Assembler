#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "assembler.h"
/*
instruct.c is hundeling lines after founded as instruction lines and generate the code for first pass function
*/
static bool put_data(char *line, int i, int *code_array, int *dc);
static bool put_string(char *line, int i, int *code_array, int *dc);
static bool put_struct(char *line, int i, int *code_array, int *dc);
/*
gets instruction line and send it to the suited function according to the data type
@param	line	instruction line
@param	i	pointer to index in the line
@param	type	the type of data
@param	code_array
@param	dc	data counter
@result	insert the commands code word for first pass
@return TRUE if all okey FALSE if problem found
*/
bool insert_data(char *line, int i, int type, int *code_array, int *dc)
{
	bool test;
	switch(type){
		case DATA: test=put_data(line, i, code_array, dc);break;
		case STRING: test=put_string(line, i, code_array, dc);break;
		case STRUCT: test=put_struct(line, i, code_array, dc);break;
	}
	return test;
}
/*
hundle data. instruction line, generate and insert the data code in code_array
@param	line	command line
@param	i	pointer to index in the line
@param	code_array
@param	dc	data counter
@result	insert the data. line code words
@return TRUE if all okey FALSE if problem found
*/
static bool put_data(char *line, int i, int *code_array, int *dc)
{
	char num[MAX_CHAR_BIT+1];
	int j , int_num;
	while (line[i]!='\n' && i<strlen(line)){
		for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
		if(line[i]=='-' || line[i]=='+' || isdigit(line[i])){
			num[0]=line[i++];
			for(j=1;j<MAX_CHAR_BIT && isdigit(line[i]);j++)
				num[j]=line[i++];
			num[j]='\0';
			if(i<strlen(line) && line[i]!=',' && line[i]!=' ' && line[i]!='\n'){ 
				printf("ERROR:invalid operand in data instruction: \"%s%c\"\n", num, line[i]);
				return FALSE;
			}
			else{
				int_num=atoi(num);
				if (int_num>S10BIT-1 || int_num<-S10BIT){/*checks that num is in 10 bits signed range*/
					printf("ERROR:data value exceed 10 bits: \"%s\"\n", num);
					return FALSE;
				}
				code_array[(*dc)--]=int_num; /*insert data*/
				for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
				if(line[i]==','){
					i++;
					for(;line[i]=='\t' || line[i]==' ';i++);
					if(line[i]=='\n' || i==strlen(line)){
						printf("ERROR:extre comma in end of instruction line\n");
						return FALSE;
					}
				}	
			}
		}
		else{
			printf("ERROR:invalid character:\"%c\" , operand can start in + , - or digit \n", line[i]);
			return FALSE;
		}		
	}
	return TRUE;
}
/*
hundle string. instruction line, generate and insert the characters code in code_array
@param	line	command line
@param	i	pointer to index in the line
@param	code_array
@param	dc	data counter
@result	insert the string. line code words
@return TRUE if all okey FALSE if problem found
*/
static bool put_string(char *line, int i, int *code_array, int *dc)
{
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if (line[i]!='"'){
		printf("ERROR: invalid string operand: need to start with quotation marks \n");
		return FALSE;
	}
	i++;
	for (;line[i]!='\n' && line[i]!='\0' && line[i]!='"'; i++)
		code_array[(*dc)--]=line[i];
	if (line[i]!='"'){
		printf("ERROR: missing quotation marks in end of string operand \n");
		return FALSE;
	}
	i++;
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if (line[i]!='\n' && line[i]!='\0'){
		printf("ERROR: extra characters after string operand\n");
		return FALSE;
	}
	code_array[(*dc)--]='\0';
	return TRUE;
}
/*
hundle struct. instruction line, generate and insert the data and characters code in code_array
@param	line	command line
@param	i	pointer to index in the line
@param	code_array
@param	dc	data counter
@result	insert the data. and string. line code words according to struct. format
@return TRUE if all okey FALSE if problem found
*/
static bool put_struct(char *line, int i, int *code_array, int *dc) 
{
	char num[MAX_CHAR_BIT+1];
	int j , int_num;
	bool test;
	for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
	if(line[i]=='-' || line[i]=='+' || isdigit(line[i])){
		num[0]=line[i++];
		for(j=1;j<MAX_CHAR_BIT && isdigit(line[i]);j++)
			num[j]=line[i++];
		num[j]='\0';
		if(i<strlen(line) && line[i]!=',' && line[i]!=' ' && line[i]!='\n'){
			printf("ERROR: invalid first operand in struct instruction: \"%s%c\"\n", num, line[i]);
			return FALSE;
		}
		else{
			int_num=atoi(num);
			if (int_num>S10BIT-1 || int_num<-S10BIT){/*checks that num is in 10 bits signed range*/
				printf("ERROR:data value in struct exceed 10 bits: \"%s\"\n", num);
				return FALSE;
			}
			code_array[(*dc)--]=int_num; /*insert data*/
			for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
			if(line[i++]!=','){
				printf("ERROR: missing comma between operands in struct\n");
				return FALSE;
			}
		}	
	}
	else{
		printf("ERROR:invalid character:\"%c\" , operand can start in + , - or digit \n", line[i]);
		return FALSE;
	}
	test=put_string(line, i, code_array, dc);
	return test;
}
