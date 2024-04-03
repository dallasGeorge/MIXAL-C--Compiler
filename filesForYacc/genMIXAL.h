#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 100

char* mixFile = "assembly.mix";
char vars[500][MAX_LINE_LENGTH];
int varsCount = 0;
char equals[3000][MAX_LINE_LENGTH];
int varsToPrintCount = 0;
int varEqualCount = 0;
int isVarDeclared(char* var){
    for(int i=0;i<varsCount;i++){
        if(strcmp(vars[i],var)==0){
            return 1;
        }
    }
    return 0;
}
void addVar(char* var){
    strcpy(vars[varsCount],var);
    varsCount++;
}

void addVarEq(char* var){
    strcpy(equals[varEqualCount],var);
    varEqualCount++;
}

void genMIXAL(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filename);
        return;
    }

    FILE *mixClean = fopen(mixFile, "w");
    if (mixClean == NULL){
        printf("Could not open file %s",mixFile);
        return;
    }
    fprintf(mixClean, "");
    fclose(mixClean);

    FILE *mix = fopen(mixFile, "a");
    if (mix == NULL){
        printf("Could not open file %s",mixFile);
        return;
    }
    fprintf(mix,"LINE CON 45\n");
    fprintf(mix," ORIG 1000\n"); 
    fprintf(mix,"START NOP\n");

    char line[256]; // Adjust the buffer size as needed
    // Read each line from the file until the end
    while (fgets(line, sizeof(line), file)) {
        // Process the line (You can do further parsing here if needed)
        char* linecopy = strdup(line);
        char* token = strtok(linecopy, "  :");
        if (token[0] == 'L') {
            fprintf(mix,"%s NOP\n", token);
        }
        else if(strcmp(token,"if")==0){
            token = strtok(NULL, " ()");
            if(token[0]=='!'){
            token[0]=' ';
            fprintf(mix," LDA %s\n CMPA ZERO\n", token);
            token = strtok(NULL, " ()!");
            token = strtok(NULL, " ()!");
            fprintf(mix," JE %s", token);}
            else{
            fprintf(mix," LDA %s\n CMPA =1=\n", token);
            token = strtok(NULL, " ()!");
            token = strtok(NULL, " ()!");
            fprintf(mix," JE %s", token);}
        }
        else if(strcmp(token,"goto")==0){
            token = strtok(NULL, " ");
            fprintf(mix," JMP %s", token);
        }
        else if(strcmp(token,"PRINT")==0){
            //if second part not number
            token = strtok(NULL, " \n");
            if(!isdigit(token[0])){
                fprintf(mix," LDA %s\n JMP PRINT\n",token);
                }
            else{//if number
                fprintf(mix," LDA =%s=\n JMP PRINT\n",token);
            }
        }
        else{
            char tokenbeforeeq[256];
            strcpy(tokenbeforeeq,token);
            char tokenaftereq[256];
            token = strtok(NULL, " =\n");
            strcpy(tokenaftereq,token);
            token = strtok(NULL, " \n");
            if(token==NULL){
                if(isdigit(tokenaftereq[0])){// var = num
                    if(!isVarDeclared(tokenbeforeeq)){//decleration
                    char str[MAX_LINE_LENGTH];
                    sprintf(str,"%s CON %s\n",tokenbeforeeq,tokenaftereq);
                    addVarEq(str);
                    addVar(tokenbeforeeq);
                    }
                    else{//not declelaration
                        fprintf(mix," LDA =%s=\n STA %s\n",tokenaftereq,tokenbeforeeq);
                    }
                }else{
                    if(tokenaftereq[0]=='!'){
                        if(isdigit(tokenaftereq[1])){ //var = !num
                            if(!isVarDeclared(tokenbeforeeq)){//decleration
                                char str[MAX_LINE_LENGTH];
                                tokenaftereq[0]=' ';
                                sprintf(str,"%s CON %s\n",tokenbeforeeq,tokenaftereq);
                                fprintf(mix," LDA %s\n CMPA ZERO\n JE NOTCHCKZE\n JNE NOTCHCKOT\n STA %s\n",tokenbeforeeq);
                                addVarEq(str);
                                addVar(tokenbeforeeq);
                            }
                            else{//not declelration
                                tokenaftereq[0]=' ';
                                fprintf(mix," LDA =%s=\n STA %s\n",tokenaftereq,tokenbeforeeq);
                                fprintf(mix," LDA %s\n CMPA ZERO\n JE NOTCHCKZE\n JNE NOTCHCKOT\n STA %s\n",tokenbeforeeq);
                            }
                        }
                        else{ //var = !var
                            if(!isVarDeclared(tokenbeforeeq)){ //decleration
                                sprintf(str,"%s CON 0\n",tokenbeforeeq);
                                addVarEq(str);
                            }
                            tokenaftereq[0]=' ';
                            fprintf(mix," LDA %s\n STA %s\n",tokenaftereq,tokenbeforeeq);
                            fprintf(mix," LDA %s\n CMPA ZERO\n JE NOTCHCKZE\n JNE NOTCHCKOT\n STA %s\n",tokenbeforeeq);
                        }
                    }
                    else if(isalpha(tokenaftereq[0])!=0){ //var = var
                        if(!isVarDeclared(tokenbeforeeq)){ //decl
                            sprintf(str,"%s CON 0\n",tokenbeforeeq);
                            addVarEq(str);
                            fprintf(mix," LDA %s\n STA %s\n",tokenaftereq,tokenbeforeeq);}
                        else{//not decl
                            fprintf(mix," LDA %s\n STA %s\n",tokenaftereq,tokenbeforeeq);
                        }
                    }
                    else if(tokenaftereq[0]=='-'){
                        if(isdigit(tokenaftereq[1])){ //var = -num
                            if(!isVarDeclared(tokenbeforeeq)){//decleration
                                char str[MAX_LINE_LENGTH];
                                sprintf(str,"%s CON %s\n",tokenbeforeeq,tokenaftereq);
                                addVarEq(str);
                                addVar(tokenbeforeeq);
                            }
                            else{//not declelaration
                                fprintf(mix," LDAN %s\n STA %s\n",tokenaftereq,tokenbeforeeq);
                            }
                        }
                        else{ //var = -var
                            if(!isVarDeclared(tokenbeforeeq)){ //decl
                                char str[MAX_LINE_LENGTH];
                                sprintf(str,"%s CON 0\n",tokenbeforeeq);
                                addVarEq(str);
                            }
                            tokenaftereq[0]=' ';
                            fprintf(mix," LDAN %s\n STA %s\n",tokenaftereq,tokenbeforeeq);
                        }
                    }
                }
            }
            else{
            
            char varBeforeEq[100];
            char varAfterEq[100];
            char op[4];
            char varAfterOp[100];
            char* token;
            token = strtok(line, " \n");
            strcpy(varBeforeEq,token);
            token = strtok(NULL, " =\n");
            strcpy(varAfterEq,token);
            token = strtok(NULL, " \n");
            strcpy(op,token);
            token = strtok(NULL, " \n");
            strcpy(varAfterOp,token);
            //printf("%s",line);
            //printf("varBeforeEq: %s\n",varBeforeEq);
            //printf("varAfterEq: %s\n",varAfterEq);
            //printf("op: %s\n",op);
            //printf("varAfterOp: %s\n",varAfterOp);
            if(!isVarDeclared(varBeforeEq)){
                sprintf(str,"%s CON 0\n",varBeforeEq);
                addVarEq(str);
            }
            if(strcmp(op,"+")==0){
                // load zero add both save to varbeforeeq
                fprintf(mix," LDA ZERO\n");
                if(isdigit(varAfterEq[0])){
                    fprintf(mix," ADD =%s=\n JOV OVERFLOW\n",varAfterEq);
                }
                else{
                    fprintf(mix," ADD %s\n JOV OVERFLOW\n",varAfterEq);
                }
                if(isdigit(varAfterOp[0])){
                    fprintf(mix," ADD =%s=\n JOV OVERFLOW\n",varAfterOp);
                }
                else{
                    fprintf(mix," ADD %s\n JOV OVERFLOW\n",varAfterOp);
                }
                fprintf(mix," STA %s\n",varBeforeEq);
            }
            else if(strcmp(op,"-")==0){
                fprintf(mix," LDA ZERO\n");
                if(isdigit(varAfterEq[0])){
                    fprintf(mix," ADD =%s=\n JOV OVERFLOW\n",varAfterEq);
                }
                else{
                    fprintf(mix," ADD %s\n JOV OVERFLOW\n",varAfterEq);
                }
                if(isdigit(varAfterOp[0])){
                    fprintf(mix," SUB =%s=\n JOV OVERFLOW\n",varAfterOp);
                }
                else{
                    fprintf(mix," SUB %s\n JOV OVERFLOW\n",varAfterOp);
                }
                fprintf(mix," STA %s\n",varBeforeEq);
            }
            else if(strcmp(op,"*")==0){
                if(isdigit(varAfterEq[0])){ //var= num* smthing
                    if(isdigit(varAfterOp[0])){ //var = num * num
                        fprintf(mix," LDA =%s=\n MUL =%s=\n JANZ OVERFLOW\n STX %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num * var
                        fprintf(mix," LDA =%s=\n MUL %s\n JANZ OVERFLOW\n STX %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    
                }
                else{ //var = var * smthing
                    if(isdigit(varAfterOp[0])){ //var = var * num
                        fprintf(mix," LDA %s\n MUL =%s=\n JANZ OVERFLOW\n STX %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var * var
                        fprintf(mix," LDA %s\n MUL %s\n JANZ OVERFLOW\n STX %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }
            }
            else if(strcmp(op,"/")==0){
                if(isdigit(varAfterEq[0])){ //var= num/ smthing
                    if(isdigit(varAfterOp[0])){ //var = num / num
                        fprintf(mix," ENTX %s\n ENTA 0\n DIV =%s=\n JOV OVERFLOW\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num / var
                        fprintf(mix," ENTX %s\n ENTA 0\n DIV %s\n JOV OVERFLOW\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    
                }
                else{ //var = var / smthing
                    if(isdigit(varAfterOp[0])){ //var = var / num
                        fprintf(mix," LDX %s\n ENTA 0\n DIV =%s=\n JOV OVERFLOW\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var / var
                        fprintf(mix," LDX %s\n ENTA 0\n DIV %s\n JOV OVERFLOW\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }
            }
            else if(strcmp(op,"%")==0){
                if(isdigit(varAfterEq[0])){ //var= num% smthing
                    if(isdigit(varAfterOp[0])){ //var = num % num
                        fprintf(mix," ENTX %s\n ENTA 0\n DIV =%s=\n JOV OVERFLOW\n STX %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num % var
                        fprintf(mix," ENTX %s\n ENTA 0\n DIV %s\n JOV OVERFLOW\n STX %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    
                }
                else{ //var = var % smthing
                    if(isdigit(varAfterOp[0])){ //var = var % num
                        fprintf(mix," LDX %s\n ENTA 0\n DIV =%s=\n JOV OVERFLOW\n STX %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var % var
                        fprintf(mix," LDX %s\n ENTA 0\n DIV %s\n JOV OVERFLOW\n STX %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }
            }
            else if(strcmp(op,"==")==0){
                if(isdigit(varAfterEq[0])){ //var= num==smthing
                    if(isdigit(varAfterOp[0])){ //var = num == num
                        fprintf(mix," LDA =%s=\n CMPA =%s=\n JE IFTRUE\n JNE IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num == var
                        fprintf(mix," LDA =%s=\n CMPA %s\n JE IFTRUE\n JNE IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    
                }
                else{ //var = var == smthing
                    if(isdigit(varAfterOp[0])){ //var = var == num
                        fprintf(mix," LDA %s\n CMPA =%s=\n JE IFTRUE\n JNE IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var == var
                        fprintf(mix," LDA %s\n CMPA %s\n JE IFTRUE\n JNE IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }              
            }
            else if(strcmp(op,"!=")==0){
                if(isdigit(varAfterEq[0])){ //var= num!=smthing
                    if(isdigit(varAfterOp[0])){ //var = num != num
                        fprintf(mix," LDA =%s=\n CMPA =%s=\n JNE IFTRUE\n JE IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num != var
                        fprintf(mix," LDA =%s=\n CMPA %s\n JNE IFTRUE\n JE IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }  
                }
                else{ //var = var != smthing
                    if(isdigit(varAfterOp[0])){ //var = var != num
                        fprintf(mix," LDA %s\n CMPA =%s=\n JNE IFTRUE\n JE IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var != var
                        fprintf(mix," LDA %s\n CMPA %s\n JNE IFTRUE\n JE IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }
            }
            else if(strcmp(op,"<")==0){
                if(isdigit(varAfterEq[0])){ //var= num<smthing
                    if(isdigit(varAfterOp[0])){ //var = num < num
                        fprintf(mix," LDA =%s=\n CMPA =%s=\n JL IFTRUE\n JGE IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num < var
                        fprintf(mix," LDA =%s=\n CMPA %s\n JL IFTRUE\n JGE IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }       
                }
                else{ //var = var < smthing
                    if(isdigit(varAfterOp[0])){ //var = var < num
                        fprintf(mix," LDA %s\n CMPA =%s=\n JL IFTRUE\n JGE IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var < var
                        fprintf(mix," LDA %s\n CMPA %s\n JL IFTRUE\n JGE IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }   
            }
            else if(strcmp(op,">")==0){
                if(isdigit(varAfterEq[0])){ //var= num>smthing
                    if(isdigit(varAfterOp[0])){ //var = num > num
                        fprintf(mix," LDA =%s=\n CMPA =%s=\n JG IFTRUE\n JLE IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num > var
                        fprintf(mix," LDA =%s=\n CMPA %s\n JG IFTRUE\n JLE IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }    
                }
                else{ //var = var > smthing
                    if(isdigit(varAfterOp[0])){ //var = var > num
                        fprintf(mix," LDA %s\n CMPA =%s=\n JG IFTRUE\n JLE IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var > var
                        fprintf(mix," LDA %s\n CMPA %s\n JG IFTRUE\n JLE IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);

                    }   
                }    
            }
            else if(strcmp(op,"<=")==0){
                if(isdigit(varAfterEq[0])){ //var= num<=smthing
                    if(isdigit(varAfterOp[0])){ //var = num <= num
                        fprintf(mix," LDA =%s=\n CMPA =%s=\n JLE IFTRUE\n JG IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num <= var
                        fprintf(mix," LDA =%s=\n CMPA %s\n JLE IFTRUE\n JG IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                }
                else{ //var = var <= smthing
                    if(isdigit(varAfterOp[0])){ //var = var <= num
                        fprintf(mix," LDA %s\n CMPA =%s=\n JLE IFTRUE\n JG IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var <= var
                        fprintf(mix," LDA %s\n CMPA %s\n JLE IFTRUE\n JG IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }      
            }
            else if(strcmp(op,">=")==0){
                if(isdigit(varAfterEq[0])){ //var= num>=smthing
                    if(isdigit(varAfterOp[0])){ //var = num >= num
                        fprintf(mix," LDA =%s=\n CMPA =%s=\n JGE IFTRUE\n JL IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num >= var
                        fprintf(mix," LDA =%s=\n CMPA %s\n JGE IFTRUE\n JL IFFALSE\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }       
                }
                else{ //var = var >= smthing
                    if(isdigit(varAfterOp[0])){ //var = var >= num
                        fprintf(mix," LDA %s\n CMPA =%s=\n JGE IFTRUE\n JL IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var >= var
                        fprintf(mix," LDA %s\n CMPA %s\n JGE IFTRUE\n JL IFFALSE\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }
            }
            else if(strcmp(op,"&&")==0){
                if(isdigit(varAfterEq[0])){ //var= num&&smthing
                    if(isdigit(varAfterOp[0])){ //var = num && num)
                        fprintf(mix," LDA =%s=\n CMPA ZERO\n LDA =%s=\n JE IFFALSE\n JNE ANDOR\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num && var
                        fprintf(mix," LDA =%s=\n CMPA ZERO\n LDA %s\n JE IFFALSE\n JNE ANDOR\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    
                }
                else{ //var = var && smthing
                    if(isdigit(varAfterOp[0])){ //var = var && num
                        fprintf(mix," LDA %s\n CMPA ZERO\n LDA =%s=\n JE IFFALSE\n JNE ANDOR\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var && var
                        fprintf(mix," LDA %s\n CMPA ZERO\n LDA %s\n JE IFFALSE\n JNE ANDOR\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }
            }
            else if(strcmp(op,"||")==0){
                if(isdigit(varAfterEq[0])){ //var= num||smthing
                    if(isdigit(varAfterOp[0])){ //var = num || num
                        fprintf(mix," LDA =%s=\n CMPA ZERO\n LDA =%s=\n JNE IFTRUE\n JE ANDOR\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = num % var
                        fprintf(mix," LDA =%s=\n CMPA ZERO\n LDA %s\n JNE IFTRUE\n JE ANDOR\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }             
                }
                else{ //var = var || smthing
                    if(isdigit(varAfterOp[0])){ //var = var || num
                        fprintf(mix," LDA %s\n CMPA ZERO\n LDA =%s=\n JNE IFTRUE\n JE ANDOR\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                    else{ //var = var || var
                        fprintf(mix," LDA %s\n CMPA ZERO\n LDA %s\n JNE IFTRUE\n JE ANDOR\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }
            }
            else if(strcmp(op,"<<")==0){
                if(isdigit(varAfterEq[0])){ //var= num<<NUM
                    if(isdigit(varAfterOp[0])){ //var = num << num
                        fprintf(mix," LDA =0=\n LDX =%s=\n SLB %s\n JANZ OVERFLOW\n STX %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }
      
                }
                else{ //var = var << NUM
                    if(isdigit(varAfterOp[0])){ //var = var << num
                        fprintf(mix," LDA =0=\n LDX %s\n SLB %s\n JANZ OVERFLOW\n STX %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
   
                }
            }
            else if(strcmp(op,">>")==0){
                if(isdigit(varAfterEq[0])){ //var= num>>smthing
                    if(isdigit(varAfterOp[0])){ //var = num >> num
                        fprintf(mix," LDX =0=\n LDA =%s=\n SRB %s\n JOV OVERFLOW\n STA %s\n",tokenaftereq,varAfterOp,varBeforeEq);
                    }     
                }
                else{ //var = var >> smthing
                    if(isdigit(varAfterOp[0])){ //var = var >> num
                        fprintf(mix," LDX =0=\n LDA %s\n SRB %s\n JOV OVERFLOW\n STA %s\n",varAfterEq,varAfterOp,varBeforeEq);
                    }
                }
            }
        }
    }
    }
    fprintf(mix,"*---------------END-----\n");
    fprintf(mix,"ENDPR HLT\n");
    fprintf(mix,"*---------------VARS-----\n");
    fprintf(mix,"ZERO CON 0\n");
    for(int i=0;i<varEqualCount;i++){
        char * token;
        char equalsfortk[256];
        strcpy(equalsfortk,equals[i]);
        token = strtok(equalsfortk, " \n");
        fprintf(mix,"%s",equals[i]);
    }
    fprintf(mix,"*---------------OVFLJMP-----\n");
    fprintf(mix,"OVERFLOW NOP\n");
    fprintf(mix," OUT ERR(18)\n");
    fprintf(mix," JBUS *(18)\n");
    fprintf(mix," JMP ENDPR\n");
    fprintf(mix,"*--------!OP-----------\nNOTCHCKZE STJ EXITNTZER\n LDA ZERO\n");
    fprintf(mix," ADD =1=\nEXITNTZER JMP *\nNOTCHCKOT STJ EXITNTOT\n LDA ZERO\nEXITNTOT JMP *\n");
    fprintf(mix,"*---------------PRINT-----\n");
    fprintf(mix,"PRINT STJ EXITPRINT\n");
    fprintf(mix," CHAR\n");
    fprintf(mix," STA  LINE+1\n");
    fprintf(mix," STX  LINE+2\n");
    fprintf(mix," JAN NEG\n");
    fprintf(mix," JMP POSSIGN\n");
    fprintf(mix,"CONTIPR OUT  LINE(18)\n");
    fprintf(mix," JBUS *(18)\n");
    fprintf(mix,"EXITPRINT JMP *\n");
    fprintf(mix,"NEG NOP\n");
    fprintf(mix,"NEGSIGN CON 45\n");
    fprintf(mix," LDA NEGSIGN\n");
    fprintf(mix," STA LINE\n");
    fprintf(mix," JMP  CONTIPR\n");
    fprintf(mix,"POSSIGN STJ EXITPOS\n");
    fprintf(mix," LDA ZERO\n");
    fprintf(mix," ADD =44=\n");
    fprintf(mix," STA LINE\n");
    fprintf(mix,"EXITPOS JMP *\n");
    fprintf(mix,"*---------------ENDOFPRINT-----\n");
    fprintf(mix,"*---------------IF-----\n");
    fprintf(mix,"IFTRUE STJ EXITTRUE\n");
    fprintf(mix," ENTA 1\n");
    fprintf(mix,"EXITTRUE JMP *\n");
    fprintf(mix,"IFFALSE STJ EXITFALSE\n");
    fprintf(mix," ENTA 0\n");
    fprintf(mix,"EXITFALSE JMP *\n");
    fprintf(mix,"*---------------ENDOFIF-----\n");
    fprintf(mix,"*-------ANDOR-------\n");
    fprintf(mix,"ANDOR STJ EXITANDOR\n");
    fprintf(mix," CMPA ZERO\n");
    fprintf(mix," JNE IFTRUE\n");
    fprintf(mix,"EXITANDOR JMP *\n");
    fprintf(mix,"*--------ENDANDOR---------\n"); 
    fprintf(mix,"*--------ERROR---------\n");
    fprintf(mix,"ERR ALF AN OV\n");
    fprintf(mix," ALF ERFLO\n");
    fprintf(mix," ALF W OCC\n");
    fprintf(mix," ALF URED \n");
    fprintf(mix," ALF OR DI\n");
    fprintf(mix," ALF VISIO\n");
    fprintf(mix," ALF N BY \n");
    fprintf(mix," ALF ZERO.\n");
    fprintf(mix," ALF PLEAS\n");
    fprintf(mix," ALF E CHE\n");
    fprintf(mix," ALF CK YO\n");
    fprintf(mix," ALF UR CO\n");
    fprintf(mix," ALF DE AN\n");
    fprintf(mix," ALF D TRY\n");
    fprintf(mix," ALF  AGAI\n");
    fprintf(mix," ALF N.   \n");
    fprintf(mix," ORIG *+15\n");
    fprintf(mix,"*--------ENDERROR---------\n");
    fprintf(mix," END START\n");
    fclose(file);
    fclose(mix);
    printf("assembly.mix created successfully!\n");
    return ;
    }




