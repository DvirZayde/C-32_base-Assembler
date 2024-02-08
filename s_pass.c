#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

extern bool is_there_ext;
/*
second pass on the file for generating the code
@param	fd1	pointer to .am file
@param	fd2	pointer to .ext file if exsist
@param	fname	name of file
@param	code_array	array of te code to generate
@param	cc	command counter, counts command code words
@param	h	header of the label linked list
@result	code array is filled and ready for conertion to 32 bit
@result	entry labels are flaged
@result	external file is filled if external requeired
@return TRUE if all okey FALSE if problem found
*/ 
bool s_pass(FILE *fd1, FILE *fd2, char* fname, int *code_array, int cc, l_ptr *h)
{
	int i, c, type;
	long int line_index;
	char *word1, temp_line[LINE_LEN+1];
	line_index=0;
	c=0;
	while(fgets(temp_line,LINE_LEN+1,fd1) != NULL && c<=cc)
	{
		i=0;/*a pointer to the index in the reading line*/
		line_index++;
		for(;temp_line[i]=='\t' || temp_line[i]==' ';i++); /*skip white*/
		word1=read_word(temp_line, i);
		if (is_label(word1)){/*skip label if found*/
			i+=strlen(word1);
			free(word1);
			for(;temp_line[i]=='\t' || temp_line[i]==' ';i++);/*skip white*/
			word1=read_word(temp_line, i);
		}
		i+=strlen(word1);
		if((type=is_ins(word1))==ENTRY){
			if(!(add_entry(*h, temp_line, i))){
				isfail=TRUE;
				printf("ERROR:line %ld in file %s invalid entry instruction\n", line_index, fname);
			}
		}
		else if((type=is_cmd(word1))<OP_NONE){
			if(!(add_address(fd2, *h, temp_line, i, code_array, &c))){
				isfail=TRUE;
				printf("ERROR:line %ld in file %s invalid operand\n", line_index, fname);
			}
		}
		free(word1);
	}
	if (isfail)/*checks that don't exceed max ram in computer*/
		return FALSE;
	return TRUE;
}
