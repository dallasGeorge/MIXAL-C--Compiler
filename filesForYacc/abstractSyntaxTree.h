

struct node* addNode(struct node *left, struct node *right, char *name) {
  struct node *nodee = (struct node*) malloc(sizeof(struct node));
  char *stringg = (char*) malloc(strlen(name)+1);
  strcpy(stringg, name);
  nodee->left = left;
  nodee->right = right;
  nodee->name = stringg;
  return(nodee);
}




void printInorder(struct node *tree, int depth) {
    int i;
    if (tree == NULL) {
        return;
    }
    for (i = 0; i < depth; i++) {
        printf("    "); 
    }
    printf("%s\n", tree->name);
    printInorder(tree->left, depth + 1);
    printInorder(tree->right, depth + 1);
}