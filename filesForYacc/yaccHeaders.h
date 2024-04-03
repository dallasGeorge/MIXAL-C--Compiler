extern int countn;
extern char *yylvalstr;
extern char* yylvalint;

// Structure to represent the symbol table entry
struct SymbolTableAddition {
    char *id_name;
    char *data_type;
    char *type;
    int line_no;
    struct SymbolTableAddition *next;
};

struct node {
  struct node *left;
  struct node *right;
  char *name;     
};



struct node *root;

// Function to insert a new symbol into the symbol table
void insertSymbol(char *id_name, char *data_type, char *type, int line_no);

// Function to check if a symbol already exists in the symbol table
int checkExistance(char *id_name);
// Function to get the data type of a symbol from the symbol table
char* getType(char *id_name);

// Function to free the memory used by the symbol table
void freeSymbolTable();

// Function to print the symbol table
void printInorder(struct node *tree, int level);


int yyparse();
int yylex();

//definition of the symbol table
struct SymbolTableAddition *symbolTable = NULL;

struct node* addNode(struct node *left, struct node *right, char *name);
int inside_loop = 0; 