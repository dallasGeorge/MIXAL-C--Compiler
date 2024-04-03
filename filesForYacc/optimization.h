#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define MAX_LINES 6000
int isInteger(double f)
{

    int intf = f;
 
    double temp2 = f - intf;
 

    if (temp2 > 0) {
 
        return 0;
    }

    return 1;
}

int readLinesFromFile(const char *filename, char *lines[]) {
   FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file");
        return -1;
    }
    int lineCount = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && lineCount < MAX_LINES) {
        size_t lineLength = strlen(buffer);

        
        lines[lineCount] = (char *)malloc(lineLength + 1);
        if (lines[lineCount] == NULL) {
            perror("Memory allocation error");
            fclose(file);
            return -1;
        }
        strcpy(lines[lineCount], buffer);

        lineCount++;
    }
    
    fclose(file);
    return lineCount;
}

void optimize(){
    const char *filename = "threeAddressCode.txt";
    char *lines[MAX_LINES];
    char str[MAX_LINE_LENGTH];
    int lineCount = readLinesFromFile(filename, lines);
    if (lineCount == -1) {
        return ;
    }

    for (int i = 0; i < lineCount; i++) {
        
        //algebric simplification
        if(strstr(lines[i],"+ 0\n") || strstr(lines[i],"- 0\n") || strstr(lines[i],"* 1\n") || strstr(lines[i],"/ 1\n") ){
            lines[i] = "";

        }
        else if (strstr(lines[i],"* 0\n") ){
            char *temp = strtok(lines[i], "=");
            lines[i] = strcat(temp, "= 0\n");
 
        }

        if(strstr(lines[i], "*")){
            //if x*num and num is power of 2 -> x << a where a = log2(num)
            char tempstr[MAX_LINE_LENGTH];
            strcpy(tempstr, lines[i]);
            char *temp = strtok(tempstr, "=");
            char *temp2 = strtok(NULL, "=");
            char *temp3 = strtok(temp2, " ");
            char *temp4 = strtok(NULL, "* \n");
            if(isdigit(temp4[0])){
                long long int numbforshift = atoll(temp4);
                if(numbforshift!=0 && isInteger(log2(numbforshift))){
                
                sprintf(str,"%s=%s << %d\n",temp,temp2,(int)log2(numbforshift));
                strcpy(lines[i], str);
                }
                
            }

        }
        if(strstr(lines[i], "/")){
            //if x/num and num is power of 2 -> x >> a where a = log2(num)
            char tempstr[MAX_LINE_LENGTH];
            strcpy(tempstr, lines[i]);
            char *temp = strtok(tempstr, "=");
            char *temp2 = strtok(NULL, "=");
            char *temp3 = strtok(temp2, " ");
            char *temp4 = strtok(NULL, "/ \n");
            if(isdigit(temp4[0])){
                long long int numbforshift = atoll(temp4);
                if(numbforshift!=0 && isInteger(log2(numbforshift))){
                
                sprintf(str,"%s=%s >> %d\n",temp,temp2,(int)log2(numbforshift));
                strcpy(lines[i], str);
                }
            }
        }
        //constant folding
        if(strstr(lines[i],"+")){
            char tempstr[MAX_LINE_LENGTH];
            strcpy(tempstr, lines[i]);
            char *temp = strtok(tempstr, "=");
            char *temp2 = strtok(NULL, "=");
            char *temp3 = strtok(temp2, " ");
            char *temp4 = strtok(NULL, "+ \n");
            if(isdigit(temp3[0]) && isdigit(temp4[0])){
                long long int numb1 = atoll(temp3);
                long long int numb2 = atoll(temp4);
                if(numb1+numb2<=1073741823 && numb1+numb2>=-1073741823){
                sprintf(str,"%s=%lld\n",temp,numb1+numb2);
                strcpy(lines[i], str);}
              
            }
        }
        if(strstr(lines[i],"-")){
            char tempstr[MAX_LINE_LENGTH];
            strcpy(tempstr, lines[i]);
            char *temp = strtok(tempstr, "=");
            char *temp2 = strtok(NULL, "=");
            char *temp3 = strtok(temp2, " ");
            char *temp4 = strtok(NULL, "- \n");
            if(isdigit(temp3[0]) && isdigit(temp4[0])){
                long long int numb1 = atoll(temp3);
                long long int numb2 = atoll(temp4);
                if(numb1-numb2<=1073741823 && numb1-numb2>=-1073741823){
                sprintf(str,"%s=%lld\n",temp,numb1-numb2);
                strcpy(lines[i], str);}
            }
        }
        if(strstr(lines[i],"*")){
            char tempstr[MAX_LINE_LENGTH];
            strcpy(tempstr, lines[i]);
            char *temp = strtok(tempstr, "=");
            char *temp2 = strtok(NULL, "=");
            char *temp3 = strtok(temp2, " ");
            char *temp4 = strtok(NULL, "* \n");
            if(isdigit(temp3[0]) && isdigit(temp4[0])){
                long long int numb1 = atoll(temp3);
                long long int numb2 = atoll(temp4);
                if(numb1*numb2<=1073741823 && numb1*numb2>=-1073741823){
                sprintf(str,"%s=%lld\n",temp,numb1*numb2);
                strcpy(lines[i], str);}
            }
        }
        if(strstr(lines[i],"/")){
            char tempstr[MAX_LINE_LENGTH];
            strcpy(tempstr, lines[i]);
            char *temp = strtok(tempstr, "=");
            char *temp2 = strtok(NULL, "=");
            char *temp3 = strtok(temp2, " ");
            char *temp4 = strtok(NULL, "/ \n");
            if(isdigit(temp3[0]) && isdigit(temp4[0])){
                long long int numb1 = atoll(temp3);
                long long int numb2 = atoll(temp4);
                if(numb2!=0)
                if(numb1/numb2<=1073741823 && numb1/numb2>=-1073741823){
                sprintf(str,"%s=%lld\n",temp,numb1/numb2);
                strcpy(lines[i], str);}
            }
        }
    }  

        FILE *fp = fopen("optimizedCode.txt", "w");
        if (fp == NULL) {
            perror("Error opening file");
            return ;
        }
        for (int i = 0; i < lineCount; i++) {
            fputs(lines[i], fp);
            
        }  
        fclose(fp);
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        printf("Optimization done!\n");
        return ;
}   