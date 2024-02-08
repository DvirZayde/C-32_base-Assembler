#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

/*
Recive file name and pointer, copy the content and spread the macros in a new file with ending .am
@param	fd1	pointer to .as file to read from
@param	name	file name without ending
@result	making a new file with eding .am wich is a copy of original with macros spread
@return	name of new file with ending .am with macros spread
*/ 
char* pre_assembler(FILE *fd1, char* name )
{
	FILE *fd2;
	m_ptr h=NULL;
	bool mflag;
	long int line_index;
	int i, c;
	char *fname, *macro, *word1, *word2, temp_line[LINE_LEN+1];
	fname=strcombine(name, ".am");
	fd2=fopen(fname,"w+");
	mflag=FALSE;
	line_index=0;
	while(fgets(temp_line,LINE_LEN+1,fd1) != NULL)
	{
		i=0;/*a pointer to the index in the reading line*/
		line_index++;
		if((strchr(temp_line,'\n')==NULL && !feof(fd1)) )/*is the line too long*/
		{
			printf("ERROR:line %ld in file \"%s\" is too long\n", line_index, name);
			isfail=TRUE;
			while((c=fgetc(fd1))!='\n' && !feof(fd1));/*skip whats left from long line*/
		}
		else if(!mflag)/*Not during MACRO*/
		{
			for(;temp_line[i]=='\t' || temp_line[i]==' ';i++);/*skip white*/
			word1=read_word(temp_line, i);
			if ((macro=is_macro(h, word1)))
			{
				fputs(macro, fd2);
			}
			else if (!strcmp(word1,"macro"))
			{
				i+=strlen(word1);
				for(;temp_line[i]=='\t' || temp_line[i]==' ';i++);/*skip white*/
				word2=read_word(temp_line, i);
				i+=strlen(word2);
				if (valid_macro(word2))
					mflag=new_macro(&h, word2, temp_line, i);
				else{
					isfail=TRUE;
					printf("ERROR:line %ld in file %s invalid macro name \"%s\"\n", line_index, name, word2);
				}
				free(word2);
			}
			else{
				fputs(temp_line, fd2);/*No macro name and no new macro*/
			}
			free(word1);
		}
		else /*During macro set*/
			mflag=add_to_macro(&h, temp_line);
		
	}
	free_macro(&h);
	fclose(fd2);
	if (isfail){
		remove(fname);
		free(fname);
		return NULL;
	}
	else
		return fname;
		
}
