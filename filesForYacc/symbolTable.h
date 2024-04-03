// function that adds a symbol to the symbol table
void addSymbol(char *id_name, char *data_type, char *type, int line_no)
{
    if (!strcmp(type, "variable") && checkExistance(id_name))
    {
        printf("error at line %d: Symbol '%s' already exists in the symbol table.\n", countn, id_name);
        exit(EXIT_FAILURE);
    }
    else
    {
        struct SymbolTableAddition *newAddition = (struct SymbolTableAddition *)malloc(sizeof(struct SymbolTableAddition));
        newAddition->id_name = strdup(id_name);
        newAddition->data_type = strdup(data_type);
        newAddition->type = strdup(type);
        newAddition->line_no = line_no;
        newAddition->next = symbolTable;
        symbolTable = newAddition;
    }
}

// Function to checks existance of a symbol in the symbol table using its name
int checkExistance(char *id_name)
{
    struct SymbolTableAddition *current = symbolTable;
    while (current != NULL)
    {
        if (strcmp(current->id_name, id_name) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

char* getType(char *id_name)
{   if (checkExistance(id_name)){
    struct SymbolTableAddition *current = symbolTable;
    while (current != NULL)
    {
        if (strcmp(current->id_name, id_name) == 0)
        {
            return current->data_type;
        }
        current = current->next;
    }}
    return NULL;
}


//function that prints the symbol table
void printSymbolTable(struct SymbolTableAddition *current)
{
    if (current == NULL)
    {   printf("\n");
        printf("SYMBOL TABLE:\n");
        return;
    }

    printSymbolTable(current->next);

    printf("Name: %s, Data Type: %s, Type: %s, Line No: %d\n", current->id_name, current->data_type, current->type, current->line_no);
}



// Function to free the allocated memory used by the symbol table
void freeSymbolTable()
{
    struct SymbolTableAddition *current = symbolTable;
    while (current != NULL)
    {
        struct SymbolTableAddition *temp = current;
        current = current->next;
        free(temp->id_name);
        free(temp->data_type);
        free(temp->type);
        free(temp);
    }
}
