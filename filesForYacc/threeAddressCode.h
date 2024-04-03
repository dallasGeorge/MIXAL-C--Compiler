

#include <stdio.h>

char *threeAddressfile = "threeAddressCode.txt";

char str[100];


static unsigned long int  tempVarCount = 0;
static unsigned long int tempLabelCount = 0;
char *breakLabel=NULL;
char *continueLabel=NULL;
void appendFile(char *threeAddressfile, char *str){
    FILE *fp = fopen(threeAddressfile, "a");
    if (fp == NULL){
        printf("Could not open file %s",threeAddressfile);
        return;
    }
    fprintf(fp, "%s", str);
    fclose(fp);
}
char * setBreakLabel(char * label){
    breakLabel=label;
}
char* setContinueLabel(char * label){
    continueLabel=label;
}
char * getBreakLabel(){
    return breakLabel;
}
char * getContinueLabel(){
    return continueLabel;
}

void replaceNodeWithTempVar(struct node* node, char* tempVarName) {
    free(node->name);
    node->name = strdup(tempVarName);
    node->left = NULL;
    node->right = NULL;
}
char* generateTempVar() {
    char* tempVar = (char*)malloc(10); 
    sprintf(tempVar, "HELPV%d", tempVarCount++);
    addSymbol(tempVar, "N/A", "temporary", countn);
    return tempVar;
}

char* generateLabel() {
    char* label = (char*)malloc(10); 
    sprintf(label, "L%d", tempLabelCount++);
    addSymbol(label, "N/A", "label", countn);
    return label;
}

void generateThreeAddressCode(struct node* atNow) {
    
    if (atNow == NULL)
        return;
    if (strcmp(atNow->name,"PRINT")==0){
        generateThreeAddressCode(atNow->right);
        sprintf(str,"PRINT %s\n", atNow->right->name);
        appendFile(threeAddressfile,str);
    }
    if (strcmp(atNow->name, "DECL") == 0) {
        sprintf(str,"%s = 0\n", atNow->left->name); 
        appendFile(threeAddressfile,str);
        generateThreeAddressCode(atNow->left);
    } else if (strcmp(atNow->name, "=") == 0) {
        generateThreeAddressCode(atNow->right);
        sprintf(str,"%s %s %s\n", atNow->left->name,atNow->name, atNow->right->name);
        appendFile(threeAddressfile,str);
      
    } 
    else if (strcmp(atNow->name,"+=")==0){
      generateThreeAddressCode(atNow->right);
        sprintf(str,"%s = %s + %s\n",atNow->left->name,atNow->left->name,atNow->right->name);
        appendFile(threeAddressfile,str);
      
    } else if (strcmp(atNow->name,"-=")==0){
      generateThreeAddressCode(atNow->right);
        sprintf(str,"%s = %s - %s\n",atNow->left->name,atNow->left->name,atNow->right->name);
        appendFile(threeAddressfile,str);
       
    } else if (strcmp(atNow->name,"*=")==0){
        generateThreeAddressCode(atNow->right);
        sprintf(str,"%s = %s * %s\n",atNow->left->name,atNow->left->name,atNow->right->name);
        appendFile(threeAddressfile,str);
    } else if (strcmp(atNow->name,"/=")==0){
        generateThreeAddressCode(atNow->right);
        sprintf(str,"%s = %s / %s\n",atNow->left->name,atNow->left->name,atNow->right->name);
        appendFile(threeAddressfile,str);
          
    } else if (strcmp(atNow->name,"%=")==0){
        generateThreeAddressCode(atNow->right);
        sprintf(str,"%s = %s %% %s\n",atNow->left->name,atNow->left->name,atNow->right->name);
        appendFile(threeAddressfile,str);
           
    }
     else if (strcmp(atNow->name, "!") == 0) {
        generateThreeAddressCode(atNow->right);
        char* tempVar = generateTempVar();
        sprintf(str,"%s = !%s\n", tempVar, atNow->right->name);
        appendFile(threeAddressfile,str);
       
        replaceNodeWithTempVar(atNow, tempVar);
        free(tempVar);
    } else if (strcmp(atNow->name, "MINUSEXP") == 0) {
        generateThreeAddressCode(atNow->right);
        char* tempVar = generateTempVar();
        sprintf(str,"%s = -%s\n", tempVar, atNow->right->name);
        appendFile(threeAddressfile,str);
 
        replaceNodeWithTempVar(atNow, tempVar);
        free(tempVar);
    }
    else if (strcmp(atNow->name, "+") == 0 ||
        strcmp(atNow->name, "-") == 0 ||
        strcmp(atNow->name, "*") == 0 ||
        strcmp(atNow->name, "%") == 0 ||
        strcmp(atNow->name, "<") == 0 ||
        strcmp(atNow->name, ">") == 0 ||
        strcmp(atNow->name, "<=") == 0 ||
        strcmp(atNow->name, ">=") == 0 ||
        strcmp(atNow->name, "==") == 0 ||
        strcmp(atNow->name, "!=") == 0 ||
        strcmp(atNow->name, "||") == 0 ||
        strcmp(atNow->name, "&&") == 0 ||
        strcmp(atNow->name, "/") == 0) {
        generateThreeAddressCode(atNow->left);
        generateThreeAddressCode(atNow->right);
        if (strcmp(getType(atNow->left->name), "temporary") == 0 && strcmp(getType(atNow->right->name), "temporary") == 0) {
            // Both are temporary
            sprintf(str,"%s = %s %s %s\n", atNow->left->name, atNow->left->name, atNow->name, atNow->right->name);
            appendFile(threeAddressfile,str);
        } else if (strcmp(getType(atNow->left->name), "temporary") == 0) {
            sprintf(str,"%s = %s %s %s\n", atNow->left->name, atNow->left->name, atNow->name, atNow->right->name);
            appendFile(threeAddressfile,str);
            
            replaceNodeWithTempVar(atNow->right, atNow->left->name);
        } else if (strcmp(getType(atNow->right->name), "temporary") == 0) {
            sprintf(str,"%s = %s %s %s\n", atNow->right->name, atNow->left->name, atNow->name, atNow->right->name);
            appendFile(threeAddressfile,str);
            
            
            replaceNodeWithTempVar(atNow->right, atNow->left->name);
        } else {
            char* tempVar = generateTempVar();
            sprintf(str,"%s = %s %s %s\n", tempVar, atNow->left->name, atNow->name, atNow->right->name);
            appendFile(threeAddressfile,str);
            

          
            replaceNodeWithTempVar(atNow, tempVar);

            free(tempVar);
        }
        
    
    }
    else if (strcmp(atNow->name, "while") == 0) {

        char* whileStartLabel = generateLabel();
        setContinueLabel(whileStartLabel); 
        char* whileEndLabel = generateLabel(); 
        setBreakLabel(whileEndLabel); 
        sprintf(str,"%s:\n", whileStartLabel);

        appendFile(threeAddressfile,str);

        generateThreeAddressCode(atNow->left); // Condition expression
        
    
        sprintf(str,"if (!%s) goto %s\n", atNow->left->name, whileEndLabel); 
        appendFile(threeAddressfile,str);
        

        
        generateThreeAddressCode(atNow->right); 

     
        sprintf(str,"goto %s\n", whileStartLabel);
        appendFile(threeAddressfile,str);
      

        
        sprintf(str,"%s:\n", whileEndLabel);
        appendFile(threeAddressfile,str);
      

     
        free(whileStartLabel);
        free(whileEndLabel);
        setContinueLabel(NULL);
        setBreakLabel(NULL);
    }
 else if (strcmp(atNow->name, "if") == 0) {
        
        generateThreeAddressCode(atNow->left->left); // Condition expression
        char* ifStartLabel = generateLabel(); 
        char* elseLabel = generateLabel(); 
        char* ifEndLabel = generateLabel(); 

     
        sprintf(str,"if (!%s) goto %s\n", atNow->left->left->name, elseLabel); 
        appendFile(threeAddressfile,str);
        

        generateThreeAddressCode(atNow->left->right ); 
        sprintf(str,"goto %s\n", ifEndLabel);
        appendFile(threeAddressfile,str);
  

        
        if (atNow->right != NULL) {
            sprintf(str,"%s:\n", elseLabel);
            appendFile(threeAddressfile,str);

            generateThreeAddressCode(atNow->right); // Else block
            
        }
        else{
            sprintf(str,"%s:\n", elseLabel);
            appendFile(threeAddressfile,str);
            
            
        }
        sprintf(str,"%s:\n", ifEndLabel);
        appendFile(threeAddressfile,str);
     

        free(ifStartLabel);
        free(elseLabel);
        free(ifEndLabel);}
    else if (strcmp(atNow->name, "for") == 0) {
        
        generateThreeAddressCode(atNow->left->left); // Init
        char* forStartLabel = generateLabel(); 
        char* forEndLabel = generateLabel(); 
        char* forIncrementLabel = generateLabel();
        setBreakLabel(forEndLabel); 
        setContinueLabel(forIncrementLabel);
       
        sprintf(str,"%s:\n", forStartLabel);
        appendFile(threeAddressfile,str);


        
        generateThreeAddressCode(atNow->left->right->left); // Condition expression

 
        sprintf(str,"if (!%s) goto %s\n", atNow->left->right->left->name, forEndLabel);
        appendFile(threeAddressfile,str);



        generateThreeAddressCode(atNow->right); 

        sprintf(str,"%s:\n", forIncrementLabel);
        appendFile(threeAddressfile,str);
  

        generateThreeAddressCode(atNow->left->right->right); // Increment expression

      
        sprintf(str,"goto %s\n", forStartLabel);
        appendFile(threeAddressfile,str);


    
        sprintf(str,"%s:\n", forEndLabel);
        appendFile(threeAddressfile,str);



        free(forStartLabel);
        free(forEndLabel);
        free(forIncrementLabel);
        setContinueLabel(NULL);
        setBreakLabel(NULL);
    }
    else if (strcmp(atNow->name, "CONTINUE") == 0) {
        if (getContinueLabel() != NULL) {
            sprintf(str,"goto %s\n", getContinueLabel());
            appendFile(threeAddressfile,str);
         
        } 
    }
    else if (strcmp(atNow->name, "BREAK") == 0) {
        if (getBreakLabel() != NULL) {
            sprintf(str,"goto %s\n", getBreakLabel());
            appendFile(threeAddressfile,str);
           
        } 
    }
    
else {
        generateThreeAddressCode(atNow->left);
        generateThreeAddressCode(atNow->right);
    }
}
