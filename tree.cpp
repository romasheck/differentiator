#include "tree.h"

int TreeCtor (Tree* tree)
{
    THEAD = (Node*) calloc(1, sizeof(Node));
    if (THEAD == NULL)
    {
        /**/
    }

    tree->size = 1;
    THEAD->left_node = NULL;
    THEAD->right_node = NULL;

    return 1;
}

int NodeInsert (Node* node, node_data_t n_data, int side)
{
    Node* tmp_ptr = (Node*) calloc(1, sizeof(Node));

    if (tmp_ptr == NULL || !node)
    {
        printf ("cannt allocate new node with grand-node %p", node);
        return 0;
    }
    if (side == RIGHT_N)
    {
        RNODE = tmp_ptr;
        RNODE->n_data = n_data;
        RNODE->right_node = NULL;
        RNODE->left_node  = NULL;
    }
    if (side == LEFT_N)
    {
        LNODE = tmp_ptr;
        LNODE->n_data = n_data;
        LNODE->left_node  = NULL;
        LNODE->right_node = NULL;
    }
    if (side != LEFT_N && side != RIGHT_N)
    {
        printf("jopa side\n");
    }

    return 1;
}

int TreeDtor (Tree* tree)
{
    NodesDtor (THEAD);

    free(tree);

    return 0;
}

int NodesDtor (Node* node)
{
    if (LNODE == NULL && RNODE == NULL)
    {
        free(node);
        return 0;
    }
    if (LNODE != NULL)
    {
        NodesDtor (LNODE);
    }
    if (RNODE != NULL)
    {
        NodesDtor (RNODE);
    }

    LNODE = NULL;
    RNODE = NULL;
    free (node);

    return 0;
}

#define FPRINT(...) fprintf(log_file, __VA_ARGS__)

int TreeDump (Tree* tree, const char* log_File, const char* png_File)
{
    FILE* log_file = fopen(log_File, "w");
    if (log_file == NULL)
    {
        printf("Log file does not opened!\n");
    }
    //FPRINT("<graph>")
    FPRINT("digraph G{\n");
    FPRINT("    graph [ rankdir=UD ]\n");

    NodesIntroduceDump(THEAD, log_file);

    NodesConnectDump(THEAD, log_file);

    FPRINT("}");
    //FPRINT("</graph>")

    fclose(log_file);

    char sys_call [100] = {};
    sprintf (sys_call, "\"C:/Program Files/Graphviz/bin/dot.exe\" -Tpng %s -o %s", log_File, png_File);  
    system (sys_call);
    
    return 0;
}

int NodesIntroduceDump (Node* node, FILE* log_file)
{
    if (LNODE == NULL && RNODE == NULL)
    {
        FPRINT("    ");
        Node_IntroduceDump (node, log_file);
        FPRINT("\n");
        return 0;
    }
    if (LNODE != NULL)
    {
        NodesIntroduceDump(LNODE, log_file);
    }

    FPRINT("    ");
    Node_IntroduceDump (node, log_file);
    FPRINT("\n");

    if (RNODE != NULL)
    {
        NodesIntroduceDump(RNODE, log_file);
    }

    return 0;
}

int Node_IntroduceDump (Node* node, FILE* log_file)
{
    FPRINT("N%p [label = \"", node);
    
    switch (TYPE)
    {
    case OTHER:
    {
        fprintf (log_file, "%s", DATA.cp);
        break;
    }
    case INTEGER:
    {
        fprintf (log_file, "%d", DATA.i);
        break;
    }
    case STR:
    {
        fprintf (log_file, "%s", DATA.cp);
        break;
    }
    case VARIABLE:
    {
        //PRINT_LINE
        fprintf (log_file, "%c", DATA.c);
        break;
    }
    case BIN_OPERATOR:
    {
        fprintf (log_file, "%c", DATA.c);
        break;
    }
    case UNO_OPERATOR:
    {
        fprintf (log_file, "%c", DATA.c);
        break;
    }
    case REAL_NUM:
    {
        fprintf (log_file, "%.2f", DATA.r);
        break;
    }
    default:
    {   
        printf ("jopa with write node: %d & data: %d\n", node, DATA.i); 
        break;
    }
    }

    FPRINT("\"];");

    return 0;
}

int NodesConnectDump (Node* node, FILE* log_file)
{
    if (LNODE == NULL && RNODE == NULL)
    {
        return 0;
    }
    if (LNODE != NULL)
    {
        FPRINT("    ");
        FPRINT("N%p", node);
        FPRINT(" -> ");
        FPRINT("N%p ;", LNODE);
        FPRINT("\n");
        NodesConnectDump(LNODE, log_file);
    }
    if (RNODE != NULL)
    {
        FPRINT("    ");
        FPRINT("N%p", node);
        FPRINT(" -> ");
        FPRINT("N%p ;", RNODE);
        FPRINT("\n");
        NodesConnectDump(RNODE, log_file);
    }

    return 0;
}
/*
int Node_ConnectDump (Node* node, FILE* log_file)
{
    FPRINT("\"%p \\n DATA: ", node);
    
    switch (TYPE)
    {
    case OTHER:
    {
        fprintf (log_file, "%s", DATA.cp);
        break;
    }
    case INTEGER:
    {
        fprintf (log_file, "%d", DATA.i);
        break;
    }
    case STR:
    {
        fprintf (log_file, "%s", DATA.cp);
        break;
    }
    case VARIABLE:
    {
        fprintf (log_file, "%c", DATA.c);
        break;
    }
    case OPERATOR:
    {
        fprintf (log_file, "%c", DATA.c);
        break;
    }
    case REAL_NUM:
    {
        fprintf (log_file, "%.2f", DATA.r);
        break;
    }
    default:
    {   
        printf ("jopa with write node: %d & data: %d\n", node, DATA.i); 
        break;
    }
    }

    FPRINT("\\n LEFT = %p \\n RIGHT = %p\"[collor = red]", LNODE, RNODE);

    return 0;
}
*/

int TreeSave (Tree* tree, FILE* tree_file)
{    
    /*CHECKER */
    NodesWrite (THEAD, tree_file);
    /*CHECKER*/
    return 0;
} 

int NodesWrite (Node* node, FILE* tree_file)
{
    fprintf (tree_file, "(");

    if (RNODE == NULL && LNODE == NULL)
    {
        Node_Write (node, tree_file);
        fprintf (tree_file, ")");
        return 0;         
    }

    if (LNODE != NULL)
    {
        NodesWrite(LNODE, tree_file);
    }

    Node_Write (node, tree_file);

    if (RNODE != NULL)
    {
        NodesWrite(RNODE, tree_file);
    }

    fprintf (tree_file, ")");

    return 0;
}

int Node_Write (Node* node, FILE* tree_file)
{
    switch (TYPE)
    {
    case OTHER:
    {
        fprintf (tree_file, "%s", DATA.cp);
        break;
    }
    case INTEGER:
    {
        fprintf (tree_file, "%d", DATA.i);
        break;
    }
    case STR:
    {
        fprintf (tree_file, "%s", DATA.cp);
        break;
    }
    case VARIABLE:
    {
        fprintf (tree_file, "%c", DATA.c);
        break;
    }
    case BIN_OPERATOR:
    {
        fprintf (tree_file, "%c", DATA.c);
        break;
    }
    case UNO_OPERATOR:
    {
        fprintf (tree_file, "%c", DATA.c);
        break;
    }
    case REAL_NUM:
    {
        fprintf (tree_file, "%.2lf", DATA.r);
        break;
    }
    default:
    {   
        printf ("jopa with write node: %p & data: %lf\n", node, DATA.r); 
        break;
    }
    }

    return 0;
}

char* TreeTake (Tree* tree, FILE* tree_file)
{
    struct stat size;
    fstat(fileno(tree_file), &size);

    char* buffer = (char*) calloc(size.st_size + 2, sizeof(char));

    fread (buffer, size.st_size, sizeof(char), tree_file);
    
    char* tmp_buf = buffer;
    NodesRead(THEAD, THEAD, (uint32_t*)&tmp_buf);
    
    THEAD = THEAD->left_node;

    return buffer;
}

#define CUR_SYMBOL *(char*)(*buf_ptr)

int NodesRead (Node* node, Node* head, uint32_t* buf_ptr)//problem : cur_s = 1 and that so....
{
    if (CUR_SYMBOL == '(')
    {
        CUR_SYMBOL = '\0';
        (*buf_ptr)++;

        NodeInsert(node, {NOT_A_TYPE, 0}, LEFT_N);
        NodesRead(LNODE, head, buf_ptr);
    }

    if (node == head)
    {
        return 0;
    }

    PutDataInNode (node, buf_ptr);

    if (CUR_SYMBOL == '(')
    {
        CUR_SYMBOL = '\0';
        (*buf_ptr)++;        
        NodeInsert(node, {NOT_A_TYPE, 0}, RIGHT_N);
        NodesRead(RNODE, head, buf_ptr);
    }
    if (CUR_SYMBOL == ')')
    {
        CUR_SYMBOL = '\0';
        (*buf_ptr)++;
        return 0;
    }
    else 
    {
        printf("jopa with read %c\n", CUR_SYMBOL);
        return 1;
    }
}

int PutDataInNode (Node* node, uint32_t* buf_ptr)
{

    if (CUR_SYMBOL <= '9' && CUR_SYMBOL >= '0')
    {
        TYPE = REAL_NUM;
        double qweqwe;
        sscanf ((char*)*buf_ptr, "%lf", &qweqwe);
        DATA.r = qweqwe;
    }
    else 
    {
        TYPE = OTHER;
        DATA.cp = (char*)*buf_ptr;
    }
    while (1)
    {
        if (CUR_SYMBOL == ')' || CUR_SYMBOL == '(')
        {
            break;
        }
        
        (*buf_ptr)++;
    }
    
    return 0;
}