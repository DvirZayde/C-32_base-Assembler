#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

extern bool is_there_ext;
/*
first pass on the file for generating the code
@param	fd1	pointer to .am file
@param	fname	name of file
@param	code_array	array of te code to generate
@param	cc	command counter, counts command code words
@param	dc	data counter, counts data, string and struct code words
@param	h	header of the label linked list
@result	code array is filled excluding labels addressing in command words that requeirs it
@result	labels list is filled
@return TRUE if all okey FALSE if problem found
note: commands will fill the beginning of array "forward" and data will start in the end of array and advance "backwards"
note: func will determine if externals labels ae used
*/
bool f_pass(FILE *fd1, char* fname, int *code_array, int *cc, int *dc, l_ptr *h)
{
	bool lflag;/*check if during label initializing*/
	int i, c, type;
	long int line_index;
	char *label, *word1, temp_line[LINE_LEN+1];
	line_index=0;
	while(fgets(temp_line,LINE_LEN+1,fd1) != NULL)
	{
		lflag=FALSE; 
		i=0;/*a pointer to the index in the reading line*/
		line_index++;
		if((strchr(temp_line,'\n')==NULL && !feof(fd1)) )/*is the line too long after macro spread*/
		{
			printf("ERROR:line %ld in file \"%s\" is too long\n", line_index, fname);
			isfail=TRUE;
			while((c=fgetc(fd1))!='\n' && !feof(fd1));/*skip whats left from long line*/
		}
		else if(temp_line[0]!=';')/*skips the line if first characters is ;*/
		{
			for(;temp_line[i]=='\t' || temp_line[i]==' ';i++); /*skip white*/
			word1=read_word(temp_line, i);
			if (is_label(word1)){ /*This part checks if label is initiates*/
				if((label=valid_label(word1))){/*get label name without : , if invalid return NULL*/
					lflag=TRUE;
					
				}
				else{
					isfail=TRUE;
					printf("ERROR:line %ld in file %s invalid label name \"%s\"\n", line_index, fname, word1);
					
				}
				i+=strlen(word1)+1;/*+1 to skip ':' that removed*/
				for(;temp_line[i]=='\t' || temp_line[i]==' ';i++);/*skip white*/
				word1=read_word(temp_line, i);
			}
			i+=strlen(word1);
			if((type=is_ins(word1))<INS_NONE){
				if(type<=STRUCT){
					if(lflag)
						if (!insert_label(h, label, type, (MAX_RAM-1-*dc))){
							isfail=TRUE;
							printf("ERROR:line %ld in file %s multiple inialize of label name \"%s\"\n", line_index, fname, label);
						}
					if(!insert_data(temp_line, i, type, code_array, dc)){
						isfail=TRUE;
						printf("ERROR:line %ld in file %s invalid instruction line \"%s\"\n", line_index, fname, word1);
					}
				}
				if(type==EXTERN){
					if(!(insert_Xlabels(h, temp_line, i))){
						isfail=TRUE;
						printf("ERROR:line %ld in file %s invalid external label/s \n", line_index, fname);
					}
					is_there_ext=TRUE;/*A flag to use for opening external file later*/
				}
			}
			else if((type=is_cmd(word1))<OP_NONE){
				if(lflag)
					if (!insert_label(h, label, INS_NONE, *cc)){
						isfail=TRUE;
						printf("ERROR:line %ld in file %s multiple inialize of label name \"%s\"\n", line_index, fname, label);
					}
				if(!insert_cmd(temp_line, i, type, code_array, cc)){
					isfail=TRUE;
					printf("ERROR:line %ld in file %s invalid command line\n", line_index, fname);
				}
			}
			else if((word1[0]!='\n' && word1[0]!=EOF && word1[0]!='\0') || lflag ){/*if not a total white line*/
				isfail=TRUE;
				printf("ERROR:line %ld in file %s command/instruction word not found: \"%s\"\n", line_index, fname, word1);
			}
			if (lflag)
				free(label);
			free(word1);
		}/*big else if*/
	}/*while*/
	if (isfail || cc>(dc+1))/*checks that don't exceed max ram in computer*/
		return FALSE;
	dc_label_update(*h, *cc);
	return TRUE;
} 


