%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
char* yylvalstr;
char* yylvalint; 
int countn=1; // Define the yylineno variable to keep track of line numbers
void enterScope();
void exitScope();
%}

%%

"for"                       { 
    strcpy(yylval.node_type.name,(yytext));
    return FOR; }
"else"                      { 
    strcpy(yylval.node_type.name,(yytext));
    return ELSE; }
"if"                        { 
    strcpy(yylval.node_type.name,(yytext));
    return IF; }
"while"                     { 
    strcpy(yylval.node_type.name,(yytext));
    return WHILE; }
"continue"                  { 
    strcpy(yylval.node_type.name,(yytext));
    return CONTINUE; }
"break"                     { 
    strcpy(yylval.node_type.name,(yytext));
    return BREAK; }
"print"                     { 
    strcpy(yylval.node_type.name,(yytext));
    return PRINT; }
"int"                       { 
    strcpy(yylval.node_type.name,(yytext));
    return INT; }
"var"                       { 
    strcpy(yylval.node_type.name,(yytext));
    return VAR; }
"&&"                        { 
    strcpy(yylval.node_type.name,(yytext));
    return AND; }
"||"                        { 
    strcpy(yylval.node_type.name,(yytext));
    return OR; }
"!"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return NOT; }
"+"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return '+'; }
"-"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return '-'; }

"*"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return '*'; }
"/"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return '/'; }
"%"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return '%'; }
"<"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return '<'; }
">"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return '>'; }
"<="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return LEQ; }
">="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return GEQ; }
"=="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return EQ; }
"!="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return NEQ; }
"+="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return PLUSASSIGN;}
"-="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return MINUSASSIGN;}
"*="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return MULTASSIGN;}
"/="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return DIVASSIGN;}
"%="                        { 
    strcpy(yylval.node_type.name,(yytext));
    return MODASSIGN;}
"="                         { 
    strcpy(yylval.node_type.name,(yytext));
    return ASSIGN; }
";"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return ';'; }
"{"                         { 
   
    strcpy(yylval.node_type.name,(yytext));
    return '{'; }
"}"                         { 
    
    strcpy(yylval.node_type.name,(yytext));
    return '}'; }
"("                         {
    strcpy(yylval.node_type.name,(yytext));
     return '('; }
")"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return ')'; }
","                         {
    strcpy(yylval.node_type.name,(yytext));
     return ','; }
":"                         { 
    strcpy(yylval.node_type.name,(yytext));
    return ':'; }
([1-9]([0-9])*)|[0] {
     strcpy(yylval.node_type.name,(yytext));
     yylvalint = strdup(yytext);
     return num;
}
[a-zA-Z]([a-zA-Z]|[0-9]|"_")* {
    strcpy(yylval.node_type.name,(yytext));
     yylvalstr = strdup(yytext); // Pass the identifier value to the parser
    return id;
}   

[ \f\t]+                   { /* Ignore space and tab characters */ }
"\n"                       { countn++; /* Increment line number */ }
.                          {
    // Any other character is an error
    printf("Wrong character %s\n", yytext);
}

%%

int yywrap() {
    return 1;
}
