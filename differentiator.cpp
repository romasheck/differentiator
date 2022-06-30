#include "differentiator.h"

Tree* TakeTree (FILE* task)
{
    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    TreeCtor (tree);

    char* buffer = TakeTask (task);

    bool incorrect = GetG (buffer, tree->head);
    
    free (buffer); 
    
    if (incorrect)
    {
        printf ("lexical error!\n");
    }
    else
    {
        TreeDump (tree, "log_tree_task.txt", "TreeLogTask.png");
    }

    return tree;
}

int GoDif (Tree* tree_task, Tree* tree_answer)
{
    //PRINT_LINE
    DifNodes (THEAD(task), THEAD(answer));
    //PRINT_LINE
    return 0;
}

int DifNodes (Node* node_task, Node* node_answer)
{
    //PRINT_LINE
    if (LNODE(task) == NULL && RNODE(task) == NULL)
    {
        //PRINT_LINE
        DifLeaf (NODES);
        //PRINT_LINE
        return 0;
    }
    //PRINT_LINE
    if (TYPE(task) == BIN_OPERATOR)
    {
        DifBOp (NODES);
    }
    if (TYPE(task) == UNO_OPERATOR)
    {
        DifUOp (NODES);
    }

    return 0;
}

int DifLeaf (Node* node_task, Node* node_answer)
{
    if (TYPE(task) == REAL_NUM)
    {
        TYPE(answer) = REAL_NUM;
        DATA(answer).r = 0;
        //PRINT_LINE
        return 0;
    }
    if (TYPE(task) == VARIABLE)
    {
        TYPE(answer) = REAL_NUM;
        DATA(answer).r = 1;
        //PRINT_LINE
        return 0;
    }

    return 1;
}

int DifBOp (Node* node_task, Node* node_answer)
{
    TYPE(answer) = BIN_OPERATOR;

    NodeInsert (node_answer, {NOT_A_TYPE, 0}, LEFT_N);
    NodeInsert (node_answer, {NOT_A_TYPE, 0}, RIGHT_N);
    
    switch (DATA(task).c)
    {
        case '+':
        {
            DATA(answer).c = '+';

            DifNodes (LNODES);
            DifNodes (RNODES);

            break;
        }
        case '-':
        {
            DATA(answer).c = '-'; 

            DifNodes (LNODES);
            DifNodes (RNODES);

            break;
        }
        case '*':
        {
            DifMul (NODES);

            break;
        }
        case '/':
        {
            DifDiv (NODES);          

            break;
        }
        default:
        {
            return 1;
        }
    }

    return 0;
}

int DifMul  (Node* node_task, Node* node_answer)
{
    DATA(answer).c = '+';

    LNODE(answer)->T = BIN_OPERATOR;
    RNODE(answer)->T = BIN_OPERATOR;
    LNODE(answer)->D.c = '*';
    RNODE(answer)->D.c = '*';

    NodeInsert (LNODE(answer), {NOT_A_TYPE, 0}, LEFT_N);
    NodeInsert (LNODE(answer), {NOT_A_TYPE, 0}, RIGHT_N);
    NodeInsert (RNODE(answer), {NOT_A_TYPE, 0}, LEFT_N);
    NodeInsert (RNODE(answer), {NOT_A_TYPE, 0}, RIGHT_N);

    CopyNodes (RNODE(task), LNODE(answer)->R);
    CopyNodes (LNODE(task), RNODE(answer)->L);

    DifNodes (LNODE(task), LNODE(answer)->L);
    DifNodes (RNODE(task), RNODE(answer)->R);

    return 0;
}

int DifDiv (Node* node_task, Node* node_answer)
{
    DATA(answer).c = '/';

    LNODE(answer)->T = BIN_OPERATOR;
    RNODE(answer)->T = BIN_OPERATOR;
    LNODE(answer)->D.c = '-';
    RNODE(answer)->D.c = '*';

    NodeInsert (RNODE(answer), {NOT_A_TYPE, 0}, LEFT_N);     //denominator = r*r
    NodeInsert (RNODE(answer), {NOT_A_TYPE, 0}, RIGHT_N);

    CopyNodes (RNODE(task), RNODE(answer)->R);              
    CopyNodes (RNODE(task), RNODE(answer)->L);
    
    NodeInsert (LNODE(answer),{BIN_OPERATOR, '*'}, LEFT_N);     //numeral = (l')*r - l*(r')
    NodeInsert (LNODE(answer),{BIN_OPERATOR, '*'}, RIGHT_N);
    NodeInsert (LNODE(answer)->R,{BIN_OPERATOR, '*'}, LEFT_N);
    NodeInsert (LNODE(answer)->L,{BIN_OPERATOR, '*'}, RIGHT_N);
    NodeInsert (LNODE(answer)->L, {NOT_A_TYPE, 0}, LEFT_N);
    NodeInsert (LNODE(answer)->R, {NOT_A_TYPE, 0}, RIGHT_N);

    CopyNodes (RNODE(task), LNODE(answer)->L->R);
    CopyNodes (LNODE(task), LNODE(answer)->R->L);

    DifNodes (LNODE(task), LNODE(answer)->L->L);
    DifNodes (RNODE(task), LNODE(answer)->R->R);  

    return 0;
}

int DifUOp (Node* node_task, Node* node_answer)
{
    TYPE(answer) = BIN_OPERATOR;
    DATA(answer).c = '*';

    NodeInsert (node_answer, {NOT_A_TYPE, 0}, LEFT_N);
    DifNodes (LNODES);

    if (DATA(task).c == 's')
    {
        NodeInsert (node_answer, {UNO_OPERATOR, 'c'}, RIGHT_N);
        NodeInsert (RNODE(answer), {NOT_A_TYPE, 0}, LEFT_N);

        CopyNodes (LNODE(task), RNODE(answer)->L);
    }
    if (DATA(task).c == 'c')
    {
        NodeInsert (node_answer, {BIN_OPERATOR, '*'}, RIGHT_N);
        node_data_t tmp_data;
        tmp_data.type = REAL_NUM;
        tmp_data.data.r = -1;
        NodeInsert (RNODE(answer), tmp_data, LEFT_N);
        NodeInsert (RNODE(answer), {UNO_OPERATOR, 's'}, RIGHT_N);
        NodeInsert (RNODE(answer)->R, {NOT_A_TYPE, 0}, LEFT_N);

        CopyNodes (LNODE(task), RNODE(answer)->R->L);
    }
    
    return 0;
}

int CopyNodes (Node* node_task, Node* node_answer)
{
    if (RNODE(task) == NULL && LNODE(task) == NULL)
    {
        TYPE(answer) = TYPE(task);
        DATA(answer) = DATA(task);
        return 0;         
    }
    if (LNODE(task) != NULL)
    {
        NodeInsert(node_answer, {NOT_A_TYPE, 0}, LEFT_N);
        CopyNodes(LNODES);
    }

    TYPE(answer) = TYPE(task);
    DATA(answer) = DATA(task);

    if (RNODE(task) != NULL)
    {
        NodeInsert(node_answer, {NOT_A_TYPE, 0}, RIGHT_N);
        CopyNodes(RNODES);        
    }

    return 0;
}