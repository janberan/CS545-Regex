%{
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "parser_lib.h"
int yylex(void);
void yyerror(char *);
char* parse(char* in);
extern FILE *yyin;
%}

%union {
	char str[3];
	void* ptr;
	char* outStr;
}

%token '(' ')' '<' '>'
%token <str> TAG 
%token OR AND END
 
%type <ptr> expr 
%type <ptr> orGroup 
%type <ptr> andGroup 
%type <ptr> treeTag
%type <outStr> unit

%%
unit:	
	expr END 		{$$=handleUnit($1); return (intptr_t)$$;}
	;

expr:
	orGroup OR expr 	{ $$=handleExpr($1,$3);}
| 	orGroup 		{$$=$1;}
	;

orGroup:
	andGroup AND orGroup 	{$$=handleOrGroup($1,$3);}
|	andGroup 		{$$=$1;}
	;

andGroup:
	treeTag 		{$$=$1;}
|       '(' expr ')' 		{$$=$2;}
	;

treeTag:
	TAG 			{$$=handleTreeTag1($1);}
| 	TAG '<' expr '>' 	{$$=handleTreeTag2($1,$3);}
	;

%%

void yyerror(char *s) {
	printf("Error: %s\n", s);
}

char* parse(char* in) {
	FILE *tmp = fopen("temp.tmp", "w");
	fprintf(tmp,"%s\n",in);
	rewind(tmp);
	fclose(tmp);
	yyin = fopen("temp.tmp", "r");
	char* res = (char*)(intptr_t)yyparse();
	fclose(yyin);
	unlink("temp.tmp");
	return res;
}




