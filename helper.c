#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "assembler.h" 
/*
helper.c contains general useful public functions for all of the program's main functions 
*/

extern cmd op_cmd[]; 
extern instruction ins[];

/*
combine two strings and allocate memory for the new string
@param	str1	first string
@param	str2	second string
@return	pointer to new allocated combine string
*/
char *strcombine(char *str1, char* str2)/*V*/
{
	char *str = (char *)mallocc(strlen(str1) + strlen(str2) + 1);
	strcpy(str, str1);
	strcat(str, str2);
	return str;
}
/*
read 1 word from string (until reaching the first with character) and allocate memory for it 
@param	line	line(long string) to read the word from
@param	index	index to start from inside line
@return	pointer to new allocated word
*/
char* read_word(char* line, int index)/*V*/
{
	char *word;
	int i;
	for (i=0;line[i+index]!='\n' && line[i+index]!='\t' && line[i+index]!=' ';i++);
	word=(char*)mallocc(i+1);
	strncpy(word, (line+index), i);
	word[i]='\0';
	return word;
}
/*
like "read_word" but stop also when reaching comma character (use mostely for operands and data. reading)
@param	line	line(long string) to read the word from
@param	index	index to start from inside line
@return	pointer to new allocated word
*/
char* read_word2(char* line, int index)
{
	char *word;
	int i;
	for (i=0;line[i+index]!='\n' && line[i+index]!='\t' && line[i+index]!=' ' && line[i+index]!=',';i++);
	word=(char*)mallocc(i+1);
	strncpy(word, (line+index), i);
	word[i]='\0';
	return word;
}
/*
checks if word is an instruction word
@param	instruction	string to check if instruction word
@return	enum value(see assembler.h) matching the instruction, or INS_NONE enum if not an instruction word
*/
int is_ins(char* instruct) 
{
	int i;
	for (i=0; i<INS_NONE;i++)
		if (!(strcmp(instruct, ins[i].ins_name)))
			return i;
	return i;
}
/*
checks if word is a command word
@param	comman	string to check if command word
@return	enum value(see assembler.h) matching the command, or OP_NONE enum if not a command word
*/
int is_cmd(char* command)
{
	int i;
	for (i=0; i<OP_NONE;i++)
		if (!(strcmp(command, op_cmd[i].cmd_name)))
			return i;
	return i;
}
/*
checks that macro name is not a saved word (command or instruction word)
@param	macro	name of macro to check
@return	TRUE if not a saved word and FALSE if does
*/
bool valid_macro(char* macro)
{
	if (is_ins(macro)<INS_NONE || is_cmd(macro)<OP_NONE)
		return FALSE;
	return TRUE;
}
/*
checks if trying to initiate a label (by the ending ':')
@param	label	word to check
@return	TRUE if trying to initiate a label and FALSE if not
*/
bool is_label(char* label)
{
	if(label[strlen(label)-1]==':')
		return TRUE;
	return FALSE;
}
/*
check length of label is in limit and that is not a saved word + remove ':' from end of label
@param	label	label to check if valid
@return	pointer to label name after discarding the ending ':' or NULL if invalid label name
*/
char* valid_label(char* label)
{
	int i , l;
	label[strlen(label)-1]='\0';/*remove ':' from end of label*/
	l=strlen(label);
	if(l<=MAX_LABEL && isalpha((int)label[0])){/*checks length of label and that start with a latter*/
		for(i=1;label[i]!='\0';i++){
			if (!isalnum((int)label[i])){
				return NULL;
			}
		}
	}
	else{
		return NULL;
	}
	for (i=0; i<NUM_INS;i++)
		if (!(strcmp(label, ins[i].ins_name)))
			return NULL;
	for (i=0; i<NUM_CMD;i++)
		if (!(strcmp(label, op_cmd[i].cmd_name)))
			return NULL;
	if(label[0]=='r' && label[1]<='7' && label[1]>='0' && label[2]=='\0')/*r0 - r7*/
		return NULL;
	return label;
}
/*
gets a struct operand index access
@param	st	struct hole word
@return	operand struct access inedx 1 or 2, return -1 if invalid
*/
int get_access_index(char *st)
{
	int i, val;
	for(i=0; isalnum((int)st[i]); i++);
	if(st[i++]!='.'){
		printf("fails checking dot %c\n", st[i]);
		return -1;
	}
	val=atoi(st+i);
	if (val!=1 && val!=2){
		printf("struct access index can be 1 or 2 only\n");
		return -1;
	}
	return val;
}
/*
remove the index part from a struct name
@param	word	struct hole word
@return	pointer to struct word after discarding the index part (exp: "LOOP.2" -> "LOOP")
*/
void remove_index_struct(char *word)
{
	int i;
	for(i=0;word[i]!='.' && word[i]!='\0';i++);
	word[i]='\0';
}
/*
malloc with verification that secceed
@param	size	size to malloc
@return	pointer to allocated memory
*/
void *mallocc(long size)
{
	void *ptr = malloc(size);
	if (ptr == NULL) {
		printf("Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;
}
/*
realloc with verification that secceed
@param	size	size to realloc
@return	pointer to allocated memory
*/
void *reallocc(void *str, long size) 
{
	void *ptr = realloc(str,size);
	if (ptr == NULL) {
		printf("Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;
}

