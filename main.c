#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAPACITY 50
#define DELIM ' '

typedef struct Node {
    char *Word;
    struct Node *left;
    struct Node *right;
}Node;

Node* NewNode(char* str)
{
    Node* n = malloc(sizeof(Node));
    n->Word = malloc(CAPACITY+1);
    strcpy(n->Word,str);

    n->right= NULL;
    n->left= NULL;
    return n;
}

typedef struct {
    Node *root;
    int Size;
}BStree;

BStree* NewTree(Node* r)
{
    BStree* tree = malloc(sizeof(BStree));
    tree->root = r;
    tree->Size = 1;
    return tree;
}

int height_of_binary_tree(Node* n)
{
    if(!n)   return -2;   // the root node i inserted in the first & leafs
    else
    {
        int left_side;
        int right_side;
        left_side = height_of_binary_tree(n -> left);
        right_side = height_of_binary_tree(n -> right);
        if(left_side > right_side)
        {
            return left_side + 1;

        }
        else
            return right_side + 1;
    }
}


Node* insertBstNode1(Node* root,Node* n)
{
    if(!root) return n;

    if (strcasecmp(root->Word, n->Word) < 0)               // Root < word
        root->right = insertBstNode1(root->right, n);


    else if (strcasecmp(root->Word, n->Word) > 0)         // Root > word
        root->left = insertBstNode1(root->left, n);


    return root;
}

void insertBstNode(BStree* tree,Node* n)
{

    insertBstNode1(tree->root,n);
    (tree->Size)++;
}

void removeNewLine(char *string) {

    string[strcspn(string, "\r\n")] = 0;
}

BStree * LoadBsTree_FromFile(char nameOfFile[])
{
    FILE *f = fopen(nameOfFile,"r");

    if(!f)
    {
        printf("\nproblem in file\n");
        exit(55555);
    }

    char words[CAPACITY];
    Node* root = NewNode(" ");
    BStree* Dictionary = NewTree(root);                               // NOT SURE ABOUT IT



    while(!feof(f))
    {
        fgets(words,CAPACITY,f);
        removeNewLine(words);
        Node* n = NewNode(words);

        insertBstNode(Dictionary,n);

//        printf("\nThe size of the Dictionary= %d \n",Dictionary->Size);
    }

    printf("\nThe file loaded successfully\n");
    fclose(f);

    return Dictionary;
}

Node* Search(Node* root,char word[])
{
    if(!root) return NULL;
    if (strcasecmp(root->Word, word) == 0) return root;

    if(strcasecmp(root->Word,word)<0)
        Search(root->right,word);
    else
        Search(root->left,word);
}

Node *findMax(Node *n){
    while (n && n->right) n=n->right;
    return n;
}

Node *findMin(Node *n){
    if (n && n->left) return findMin(n->left);
    return n;

}



Node* getSuggestion1(Node* root,char* word)
{
    if(strcasecmp(root->Word,word) < 0) {
        if(root->right)
        getSuggestion1(root->right, word);
        else return root;
    }else if (strcasecmp(root->Word,word) > 0) {
        if(root->left)
        getSuggestion1(root->left, word);
        else return root;
    }
}

//Node* findPredecessor(Node* root,char word[])           // getsuggestion2
//{
//
//    Node* prec = NewNode("");
//    // base case
//    if (root == NULL) {
//        return prec;
//    }
//
//    // if a node with the desired value is found, the predecessor is the maximum
//    // value node in its left subtree (if any)
//    if (strcasecmp(root->Word,word) == 0)
//    {
//        if (root->left != NULL) {
//            return findMax(root->left);
//        }
//    }
//        // if the given key is less than the root node, recur for the left subtree
//    else if (strcasecmp(root->Word,word) > 0) {
//        return findPredecessor(root->left,  word);
//    }
//
//        // if the given key is more than the root node, recur for the right subtree
//    else {
//        // update predecessor to the current node before recursing
//        // in the right subtree
//        prec = root;
//        return findPredecessor(root->right, word);
//    }
//    return prec;
//}

Node *getPreSuccesor(Node *root,char word[]){                         //  getsuggestion3
    if (!root) return NULL;
    Node *Presuccessor = NULL;
    Node *current  = root;
    while(current && strcasecmp(current->Word,word))     // root word doesn't match the word given && root not NULL
    {
        if(strcasecmp(current->Word,word) > 0){
            Presuccessor = current;
            current= current->left;
        }
        else {
            Presuccessor = current;
            current = current->right;
        }
    }

    if(current && current->left)  // if node has left then
    {

        Presuccessor = findMax(current->left);
    }

    return Presuccessor;                      // else
}



Node *inorderSuccessor(Node *root,char word[]){                         //  getsuggestion3
    if (!root) return NULL;
    Node *successor = NULL;
    Node *current  = root;
    while(current && strcasecmp(current->Word,word))     // root word doesn't match the word given && root not NULL
    {
        if(strcasecmp(current->Word,word) > 0){
            successor = current;
            current= current->left;
        }
        else {
            successor = current;
            current = current->right;
        }
    }
    if(current && current->right)  // if node has right then
        successor = findMin(current->right);
    return successor;                      // else
}



void Check_speling(Node* root,char *word)
{
    char *token;
    token = strtok(word, " ");
    Node*n1 =NULL;
    Node*n2 =NULL;
    Node*n3 =NULL;

    /* walk through other tokens */
    while( token != NULL ) {
        if(Search(root,token) != NULL)
        {

            printf("\n%s - CORRECT\n",token);
        } else
        {

            printf("\n%s - INCORRECT, Suggestions:\t",token);
            n1 = getSuggestion1(root,token);
            printf("(1)-%s ",n1->Word);
            n2 = getPreSuccesor(root,n1->Word);
            printf("(2)-%s ",n2->Word);
            n3 = inorderSuccessor(root,n1->Word);
            printf("(3)-%s\n",n3->Word);


        }

        token = strtok(NULL, " ");
    }
}






int main() {
    BStree* Dictionary = LoadBsTree_FromFile("E:\\untitled1\\cmake-build-debug\\EN-US-Dictionary.txt");
    printf("\n-------------------------------------------------------------------------\n");
    printf("\nThe size of the Dictionary= %d \n",Dictionary->Size);
    printf("\n-------------------------------------------------------------------------\n");
    printf("\nThe height of the Dictionary= %d \n", height_of_binary_tree(Dictionary->root));
    printf("\n-------------------------------------------------------------------------\n");
    char sentence[100];
    printf("\nEnter a sentence: \n");
    gets(sentence);
    printf("\n");
    Check_speling(Dictionary->root,sentence);




    return 0;
}
