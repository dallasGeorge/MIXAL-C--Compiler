%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesForYacc/yaccheaders.h"  

%}


%union { 
	struct var_name { 
		char name[100]; 
		struct node* nd;
	} node_type; 
} 
 
%start PROGRAM
%token <node_type> id INT ':' ';' '{' '}' ',' VAR PRINT BREAK num WHILE CONTINUE IF ELSE FOR

%right <node_type>  ASSIGN PLUSASSIGN MINUSASSIGN MULTASSIGN DIVASSIGN MODASSIGN
%left <node_type>  OR
%left <node_type>  AND 
%left <node_type>  EQ NEQ
%left <node_type>  '<' '>' LEQ GEQ
%left <node_type> '+' '-'
%left <node_type>  '*' '/' '%'
%right <node_type>  NOT MINUSEXP




%nonassoc <node_type> IFX
%nonassoc <node_type> ELSE

%type <node_type> PROGRAM DECLS DECL VARS TYPE STMTS STMT SIMP CONTROL BLOCK EXP 

%%
PROGRAM : '{' DECLS STMTS '}'{
    $$.nd = addNode($2.nd, $3.nd, "PROGRAM"); 
    root = $$.nd; 
    printf("PROGRAM REACHED SUCCESFULLY IN PARSER (using bottom up, so it reached the end)\n ");};

DECLS : DECLS DECL{
    $$.nd = addNode($2.nd, $1.nd, "DECLS"); 
    }
    | {$$.nd =addNode(NULL, NULL, "");};
    ;

DECL : VAR id { addSymbol(yylvalstr, "int", "variable", countn); } VARS ':' TYPE ';' { 
    $2.nd = addNode(NULL, $4.nd, $2.name);
    $$.nd = addNode($2.nd, NULL, "DECL");
    }
    ;

VARS : ',' id { addSymbol(yylvalstr, "int", "variable", countn); } VARS { 
    $2.nd = addNode(NULL, $4.nd, $2.name);
    $$.nd = addNode($2.nd, NULL, "DECL");
   
    }
    | {$$.nd = addNode(NULL, NULL, "");};
    ;




TYPE : INT{
    $$.nd = addNode(NULL, NULL, $1.name);
    };

STMTS : STMTS STMT{
    $$.nd = addNode($1.nd, $2.nd, "STMTS");
    }
    | {$$.nd = addNode(NULL, NULL, "");};
    ;
STMT : SIMP ';' {
    $$.nd = addNode(NULL, $1.nd, "STMT");
    }
    | CONTROL {
        $$.nd = addNode(NULL, $1.nd, "STMT");
        }
    | ';' {
        $$.nd = addNode(NULL, NULL, ";");
        }
    ;
SIMP : id ASSIGN EXP {
    checkIfDeclared($1.name);
    $1.nd = addNode(NULL, NULL, $1.name);
    $$.nd = addNode($1.nd, $3.nd, "=");
    } 
    |id PLUSASSIGN EXP {
    $1.nd = addNode(NULL, NULL, $1.name);
    $$.nd = addNode($1.nd, $3.nd, "+=");
    }
    | id MINUSASSIGN EXP {
        checkIfDeclared($1.name);
        $1.nd = addNode(NULL, NULL, $1.name);
        $$.nd = addNode($1.nd, $3.nd, "-=");
        }
    | id MULTASSIGN EXP {
        checkIfDeclared($1.name);
        $1.nd = addNode(NULL, NULL, $1.name);
        $$.nd = addNode($1.nd, $3.nd, "*=");
        }
    | id DIVASSIGN EXP {
        checkIfDeclared($1.name);
        $1.nd = addNode(NULL, NULL, $1.name);
        $$.nd = addNode($1.nd, $3.nd, "/=");
        }
    | id MODASSIGN EXP { 
        checkIfDeclared($1.name);  
        $1.nd = addNode(NULL, NULL, $1.name);
        $$.nd = addNode($1.nd, $3.nd, "%=");
        }
    |PRINT EXP {
        $$.nd = addNode(NULL, $2.nd, "PRINT");
        }
;

CONTROL : IF '(' EXP ')' BLOCK %prec IFX {
    $1.nd = addNode($3.nd, $5.nd, "CONDITION");
    $$.nd = addNode($1.nd, NULL, $1.name);
    addSymbol("IF", "N/A", "keyword", countn);
    }
    |IF '(' EXP ')' BLOCK ELSE BLOCK{
        struct node *temp = addNode($3.nd, $5.nd, "CONDITION");
       
        $6.nd = addNode(NULL, $7.nd, "ELSE");
        $$.nd = addNode(temp, $6.nd, $1.name);
        addSymbol("IFELSE", "N/A", "keyword", countn);
        } 
    |WHILE { inside_loop = 1; } '(' EXP ')' BLOCK { inside_loop = 0; }{
        $$.nd = addNode($4.nd, $6.nd, $1.name);
        addSymbol("WHILE", "N/A", "keyword", countn);
       }
    |FOR '(' SIMP ';' EXP ';' SIMP ')' { inside_loop = 1; }BLOCK{ inside_loop = 0; }{
        struct node *temp = addNode($5.nd, $7.nd, "CONDITION"); 
        struct node *temp2 = addNode($3.nd, temp, "CONDITION"); 
        $$.nd = addNode(temp2, $10.nd, $1.name); 
        addSymbol("FOR", "N/A", "keyword", countn);
        }
    |CONTINUE ';' {
        if (!inside_loop){
            printf("Error: 'continue' statement outside of a loop at line: %d",countn); 
            exit(EXIT_FAILURE);
        } 
        
        $$.nd = addNode(NULL, NULL, "CONTINUE");
        addSymbol("CONTINUE", "N/A", "keyword", countn);
        }
    |BREAK ';' {
        if (!inside_loop){
            printf("Error: 'break' statement outside of a loop at line: %d",countn);
            exit(EXIT_FAILURE);
        }  
        
        $$.nd = addNode(NULL, NULL, "BREAK");
        addSymbol("BREAK", "N/A", "keyword", countn);
        }
    ;

BLOCK : STMT {
        $$.nd = addNode(NULL, $1.nd, "BLOCK");
       }
    | '{' STMTS '}'{
        $$.nd = addNode(NULL,$2.nd,"BLOCK");
       }
    ;
EXP : '('EXP')' {
    $$.nd =$2.nd;
    }
    |num  {
        $$.nd = addNode(NULL, NULL, $1.name);
        addSymbol(yylvalint, "CONST", "Constant", countn); 
        }
    |id {
        checkIfDeclared($1.name);
        $$.nd = addNode(NULL, NULL, $1.name);
        }
    |EXP '+' EXP {
        $$.nd = addNode($1.nd, $3.nd, "+");
       }
    |EXP '-' EXP {
        $$.nd = addNode($1.nd, $3.nd, "-"); 
       }
    |EXP '*' EXP {
        $$.nd = addNode($1.nd, $3.nd, "*");
       }
    |EXP '/' EXP {
        $$.nd = addNode($1.nd, $3.nd, "/");
       }
    |EXP '%' EXP {
        $$.nd = addNode($1.nd, $3.nd, "%");
       }
    |EXP '<' EXP {
        $$.nd = addNode($1.nd, $3.nd, "<");
       }
    |EXP '>' EXP {
        $$.nd = addNode($1.nd, $3.nd, ">");
       }
    |EXP LEQ EXP {
        $$.nd = addNode($1.nd, $3.nd, "<=");
       }
    |EXP GEQ EXP {
        $$.nd = addNode($1.nd, $3.nd, ">=");
       }
    |EXP EQ EXP {
        $$.nd = addNode($1.nd, $3.nd, "==");
       }
    |EXP NEQ EXP {
        $$.nd = addNode($1.nd, $3.nd, "!=");
       }
    |EXP AND EXP {
        $$.nd = addNode($1.nd, $3.nd, "&&");
       }
    |EXP OR EXP {
        $$.nd = addNode($1.nd, $3.nd, "||");
       }
    |NOT EXP {
        $$.nd = addNode(NULL, $2.nd, "!");
        }
    |'-' EXP %prec MINUSEXP{
        $$.nd = addNode(NULL, $2.nd, "MINUSEXP");
        }


%%

#include "filesForYacc/symboltable.h"
#include "filesForYacc/abstractSyntaxTree.h"
#include "filesForYacc/threeAddressCode.h"
#include "filesForYacc/genMIXAL.h"
#include "filesForYacc/optimization.h"
void yyerror(char *s)
{
   
    putc('\n', stderr);
    printf("syntax error at line: %d\n",countn);
    exit(EXIT_FAILURE);
}



void checkIfDeclared(char *str) {    
    int result = checkExistance(str);    
    if(result==0) {    
           
        printf("semantic error at line %d: Variable \"%s\" not declared before usage!\n", countn, str); 
        exit(EXIT_FAILURE);
    }
}

int main() {
    FILE *fp = fopen("threeAddressCode.txt", "w");
    fprintf(fp, "");
    fclose(fp);
    int result = yyparse();
    
    printSymbolTable(symbolTable);
    
    

    printf("\n\n Inorder traversal of the Parse Tree: \n\n");
    printInorder(root,0);
    printf("\n\n");
    generateThreeAddressCode(root);
    optimize();
    genMIXAL("optimizedCode.txt");
    return result;
}