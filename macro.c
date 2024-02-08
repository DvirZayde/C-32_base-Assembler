#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
/*
macro.c contains functions to hundle linked list of macro type
*/

extern cmd op_cmd[]; 
extern instruction ins[];

/*
Free the macro list
@param	h	head of list
*/
void free_macro(m_ptr *h)
{
	m_ptr tmp;
	while((*h)!=NULL)
	{
		free((*h)->content);
		free((*h)->name);
		tmp=(*h)->next;
		free(*h);
		(*h)=tmp;
		
	}
}
/*
Recive a new macro name and content(of first line) to copy and generate a new macro
@param	h	head of list
@param	mname	name for macro
@param	mcontent	content from first line
@param	i	index that points after macro's name
@result	a new macro is added to list
@return	TRUE if endmacro not found, FALSE if did found and finished making this macro
*/ 
bool new_macro(m_ptr *h, char *mname, char *mcontent, int i)
{
	int j, size;
	char *word;
	bool more=TRUE;
	m_ptr p;
	size=strlen(mcontent);
	p=(m_ptr)mallocc(sizeof(macro));
	(p->name)=(char*)mallocc(sizeof(mname)+1);
	strcpy(p->name,mname);
	(p->content)=(char*)mallocc(size+1);
	strcpy((p->content), (mcontent+i));
	for(j=0; (p->content)[j]!='\0'; j++){
		for(;(p->content)[j]=='\t' || (p->content)[j]==' ';j++);/*skip white*/
		word=read_word((p->content),j);
		if(!strcmp(word,"endmacro")){
			more=FALSE;
			(p->content)[j++]='\n';
			(p->content)[j]='\0';
			free(word);
			break;
		}
		else{
			j+=strlen(word);
			free(word);
		}
	}
	p->next=*h;
	(*h) = p;
	return more;
}
/*
Recive more content to add to last macro generated
@param	h	head of list
@param	mcontent	content from corrent line
@result	more content added to last macro
@return	TRUE if endmacro not found, FALSE if did found and finished making this macro
*/
bool add_to_macro(m_ptr *h, char *mcontent)
{
	int i ,msize, nsize;
	char *word;
	bool more;
	msize=strlen((*h)->content);
	nsize=strlen(mcontent);
	more=TRUE;
	(*h)->content=(char*)reallocc(((*h)->content),(nsize+msize+1));/*add place for next line*/
	strcat(((*h)->content), (mcontent));
	for(i=msize; ((*h)->content)[i]!='\0'; i++){
		for(;((*h)->content)[i]=='\t' || ((*h)->content)[i]==' ';i++);/*skip white*/
		word=read_word(((*h)->content),i);
		if(!strcmp(word,"endmacro")){
			more=FALSE;
			((*h)->content)[i++]='\n';
			((*h)->content)[i]='\0';
			free(word);
			break;
		}
		else{
			i+=strlen(word);
			free(word);
		}
	}
	return more;
}
/*
Checks if word is a name of an exisiting macro
@param	h	head of list
@param	mname	name of macro to look for
@return	pointer to content of macro, NULL if if macro name not exist
*/
char* is_macro(m_ptr h, char* mname)
{
	m_ptr tmp=h;
	while (tmp!=NULL)
	{
	if(!(strcmp(tmp->name, mname)))
		return tmp->content;
	tmp=tmp->next;
	}
	return NULL;
} 
