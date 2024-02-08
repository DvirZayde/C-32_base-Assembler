#ifndef _MAIN_HEADER
#define _MAIN_HEADER
#define MAX_LABEL 30
#define MAX_RAM 156
#define START_POINT 100 
#define LINE_LEN 80
#define CMD_LEN 3
#define INS_LEN 10
#define NUM_CMD 16
#define NUM_INS 5
#define NO_ADDR -1
#define MAX_CHAR_BIT 4 /*the maximum characters in 10 bit number*/
#define S10BIT 512 /*the max value in signed 10 bit*/
#define S8BIT 128 /*the max value in signed 8 bit*/
 
/*enum of the opcode of the different commands, OP_NONE is 16 */
typedef enum opcode {OP_MOV, OP_CMP, OP_ADD, OP_SUB, OP_NOT, OP_CLR, OP_LEA, OP_INC, OP_DEC, OP_JMP, OP_BNE, OP_GET, OP_PRN, OP_JSR, OP_RTS, OP_HLT, OP_NONE}opcode;
/*boolean type*/
typedef enum boolean { FALSE, TRUE } bool;
/*enum for the address types in command line*/
typedef enum add_type {IMMIDIATE, DIRECT, ACCESS, REGISTER} add_type;
/*enum of the ARE values*/
typedef enum are {ABSOLUTE, EXTERNAL, RELOCATABLE} are;
/*type command include the command name and it's code*/
typedef struct command {
	char cmd_name[CMD_LEN];
	int cmd_code;
}cmd;
/*type instruction include the instruct name and it's code/type that use as type from enum*/
typedef struct instruction {
	char ins_name[INS_LEN];
	int ins_code;
}instruction;
/*Define values for the different instruction type*/
typedef enum ins_type {DATA, STRING, STRUCT, EXTERN, ENTRY , INS_NONE}ins_type;
/*enum us to creat the code word the value represnt how many bits to move left to represt the code word part*/
typedef enum bitim {OPCODE=6, SREGISTER=6, DREGISTER=2, ADDS=4, ADDD=2, ARE=0, OPERAND=2, ACCINDX=2}bitim;
/*type of linked list for the macros*/
typedef struct nodem * m_ptr;/*pointer to macro type*/
typedef struct nodem
{
	char *name;/*macros name*/
	char *content;/*macros content replace the name when found*/
	m_ptr next;
	
}macro;
/*type of linked list for the labels*/
typedef struct node * l_ptr;/*pointer to label type*/
typedef struct node
{
	char name[MAX_LABEL+1];
	int type;/*data=0,string=1,struct=2, extern=3, entry=4, cmd=5*/
	bool is_entry;/*to use as flag for entry*/
	int address;/*address in the computer's HW*/
	l_ptr next;
	
}label;


extern bool isfail;

/*helper.c*/ 
bool valid_macro(char*);
char *strcombine(char *, char*);
char *read_word(char*, int);
char *read_word2(char*, int);
void *mallocc(long);
void *reallocc(void *, long);
int is_ins(char* );
int is_cmd(char* );
bool is_label(char* );
char* valid_label(char* );
int get_access_index(char *);
void remove_index_struct(char *);
/*macro.c*/
void free_macro(m_ptr *);
bool new_macro(m_ptr *, char *, char *, int);
bool add_to_macro(m_ptr *, char *);
char* is_macro(m_ptr , char*);
/*label.c*/
void free_labels(l_ptr *);
bool insert_label(l_ptr *, char *, int  , int );
bool insert_Xlabels(l_ptr *, char *, int );
void dc_label_update(l_ptr , int );
void print_label(l_ptr );
bool add_entry(l_ptr , char *, int );
/*writefile.c*/
void write_object(char *, int *, int, int);
void write_entry(char *, l_ptr);
char* convert32(int num, char *sign);
/*main program's functions*/
char* pre_assembler(FILE *, char*);
bool f_pass(FILE *, char*, int *, int *, int *, l_ptr *);
bool s_pass(FILE *,FILE *, char* , int *, int , l_ptr *);
bool insert_data(char *, int , int ,int *,int *);
bool insert_cmd(char *, int , int , int *, int *);
bool add_address(FILE *, l_ptr, char *, int , int *, int *);


#endif

