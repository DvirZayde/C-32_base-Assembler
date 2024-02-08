#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"
/*
label.c contains functions to hundle linked list of label type
*/
static bool insert_entry(l_ptr h, char* label);

/*
Free the list in the end of the program
@param	h	head of list
*/
void free_labels(l_ptr *h)
{
	l_ptr tmp;
	while(*h!=NULL)
	{
		tmp=(*h)->next;
		free(*h);
		*h=tmp;
	}
}
/*
Add new lable to list of lables and make sure name is not exist alreay
@param	h	head of list
@param	lname	name for label
@param	ltype	type for label
@param	laddress	address for label
@result	a new label added to list
@return	TRUE if added seccesfuly FALSE if found a label with same name
*/
bool insert_label(l_ptr *h, char *lname, int ltype , int laddress)
{
	l_ptr p , c;
	c=*h;
	for(;c;c=c->next)
		if(!strcmp((c->name),lname))
			return FALSE;
	p=(l_ptr)mallocc(sizeof(label));
	strcpy(p->name,lname);
	p->type=ltype;
	p->address=START_POINT+laddress;
	p->is_entry=FALSE;
	p->next=*h;
	(*h) = p;
	return TRUE;
}
/*
Insert labels from line of external instruction 
@param	h	head of list
@param	line	extern. instruction line
@param	i	index to line
@result	add all extern labels in line to the label list
@return	TRUE if added all seccesfuly, FALSE if one or more of the labels name already exist
*/
bool insert_Xlabels(l_ptr *h, char *line, int i)
{
	char* label;
	bool test;
	test=TRUE;
	while (line[i]!='\n' && i<LINE_LEN){
		for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
		label=strcat(read_word(line, i), ":");/*add ':' to the end to use the 'valid_label' function*/
		i+=strlen(label)-1;/*label length include ':' that's not in line*/
		if (strlen(label)-1==0)
			return test;
		if(!valid_label(label)){
			printf("ERROR:\"%s\" external label name invalid\n", label);
			test=FALSE;
		}
		else if(!insert_label(h, label, EXTERN, NO_ADDR)){
			printf("ERROR:\"%s\" already exist as label\n", label);
			test=FALSE;
		}
		free(label);
	}
	return test;
}
/* 
Add flag to all labels in entry. instruction line for making the entry file later
@param	h	head of list
@param	line	extern. instruction line
@param	i	index to line
@result	all the labels in entry. line are flaged ass entry labels
@return	TRUE if added all seccesfuly, FALSE if one or more of the labels name already exist
*/
bool add_entry(l_ptr h, char *line, int i)
{
	char* label;
	bool test;
	test=TRUE;
	while (line[i]!='\n' && i<LINE_LEN){
		for(;line[i]=='\t' || line[i]==' ';i++);/*skip white*/
		label=read_word(line, i);
		if (strlen(label)==0)
			return test;
		i+=strlen(label);
		if(!insert_entry(h, label)){
			printf("ERROR: entry instruct without a matching label:\"%s\"\n", label);
			test=FALSE;
		}
		free(label);
	}
	return test;
}
/*
A sub-function for add_entry that checks each label name that exist in list
@param	h	head of list
@param	label	name of label from entry. line
@return	TRUE if flaged seccusfuly, FALSE if didnt found matching label name
*/
static bool insert_entry(l_ptr h, char* label)
{
	l_ptr p;
	p=h;
	while (p!=NULL){
		if(!strcmp(p->name, label)){
			p->is_entry=TRUE;
			return TRUE;
		}
		p=p->next;
	}
	return FALSE;
}
/*
Update data labels address(counter) in the end of first pass by adding the total of commands words
@param	h	head of list
@param	cc	command counter
@result	data addresses are update to the correct location in the computer memory
*/
void dc_label_update(l_ptr h, int cc)
{
	l_ptr p;
	p=h;
	while (p!=NULL){
		if((p->type)<=STRUCT)/*Data label*/
			(p->address)+=cc;
		p=p->next;
	}
}
/*
Printing label list on standart out-put(tool for programmer, not in use)
*/
void print_label(l_ptr h)
{
	l_ptr p;
	p=h;
	while (p!=NULL){
		printf("label name:%s, num:%d\n", p->name, p->address);
		p=p->next;
	}
}

