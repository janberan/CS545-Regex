%{
#include <stdio.h>
#include "parser.h"
#include "parser_lib.h"
%}

blank		[ \t]
and		[&][&]
or		[|][|]
tag		[^ &|()<>$\n][^ &|()<>$\n]
end 		[$]

%%

{blank}+	{}

[\n]+		{}

[()<>]		{return *yytext;}
		
{or}		{
		return OR;
		}

{and}		{
		return AND;
		}


{tag}		{
		yylval.str[0] = yytext[0];
		yylval.str[1] = yytext[1];
		yylval.str[2] = '\0';
		return TAG;
		}

{end}		{
		return END;
		}

.		{
		fprintf(yyout, "ERROR TOKEN: %s\n", yytext);
		}

%%

int yywrap(void) {
	return 1;
}

