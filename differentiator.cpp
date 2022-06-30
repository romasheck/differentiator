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
    DifNodes (THEAD(task), THEAD(answer));

    return 0;
}

int DifNodes (Node* node_task, Node* node_answer)
{

    if (LNODE(task) == NULL && RNODE(task) == NULL)
    {
        DifLeaf (NODES);

        return 0;
    }

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

        return 0;
    }
    if (TYPE(task) == VARIABLE)
    {
        TYPE(answer) = REAL_NUM;
        DATA(answer).r = 1;

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
        NodeInsert (node_answer, {NOT_A_TYPE, 0}, LEFT_N);
        CopyNodes (LNODES);
    }

    TYPE(answer) = TYPE(task);
    DATA(answer) = DATA(task);

    if (RNODE(task) != NULL)
    {
        NodeInsert (node_answer, {NOT_A_TYPE, 0}, RIGHT_N);
        CopyNodes (RNODES);        
    }

    return 0;
}

int SimplifyExp (Tree* tree_exp)
{
    NodesCalc (THEAD(exp));

    return 0;
}

int NodesCalc (Node* node_exp)
{
    if (TYPE(exp) == BIN_OPERATOR)
    {
        if (TYPE(exp->L) == BIN_OPERATOR || TYPE(exp->L) == UNO_OPERATOR)
        {
            NodesCalc (node_exp->L);
        }
        if (TYPE(exp->R) == BIN_OPERATOR || TYPE(exp->R) == UNO_OPERATOR)
        {
            NodesCalc (node_exp->R);
        }

        if (TYPE(exp->L) == REAL_NUM && TYPE(exp->R) == REAL_NUM)
        {
            ArithmCalc (node_exp);

            return 0;
        }

        switch (DATA(exp).c)
        {
        case '+':
            ZeroPlus (node_exp);
            break;
        case '-':
            ZeroSub (node_exp);
            break;
        case '*':
            ZeroOneMul (node_exp);
            break;
        case '/':
            OneDiv (node_exp);
            break;
        default:
            break;
        }
    }
    else
    {
        if ((TYPE(exp->L) == BIN_OPERATOR || TYPE(exp->L) == UNO_OPERATOR) && TYPE(exp) == UNO_OPERATOR)
        {
            NodesCalc (node_exp->L);
        }   
    }

    return 0;
}

int ArithmCalc (Node* node_exp)
{
    TYPE(exp) = REAL_NUM;

    switch (DATA(exp).c)
    {
    case '+':
        DATA(exp).r = DATA(exp->L).r + DATA(exp->R).r;
        break;
    case '-':
        DATA(exp).r = DATA(exp->L).r - DATA(exp->R).r;
        break;
    case '*':
        DATA(exp).r = DATA(exp->L).r * DATA(exp->R).r;
        break;
    case '/':
        DATA(exp).r = DATA(exp->L).r / DATA(exp->R).r;
        break;
    default:
        break;
    }

    free (node_exp->L);
    free (node_exp->R);
    node_exp->L = NULL;
    node_exp->R = NULL;

    return 0;
}

#undef REPLCE_BRANCH
#define REPLACE_BRANCH(frst_side, scnd_side)\
*node_exp = *node_exp->scnd_side;         \
free(node_exp->frst_side);                \
free(node_exp->scnd_side);

int ZeroPlus (Node* node_exp)
{
    if (DATA(exp->L).r == 0)
    {
        REPLACE_BRANCH(L, R)
    }
    if (DATA(exp->R).r == 0)
    {
        REPLACE_BRANCH(R, L)
    }

    return 0;
}

int ZeroSub (Node* node_exp)
{
    if (DATA(exp->R).r == 0)
    {
       REPLACE_BRANCH(R, L)
    }

    return 0;
}

int ZeroOneMul (Node* node_exp)
{
    if (TYPE(exp->L) == REAL_NUM)
    {
        if (DATA(exp->L).r == 1)
        {
            REPLACE_BRANCH(L, R)
        }
        if (DATA(exp->L).r == 0)
        {
            REPLACE_BRANCH(R, L) 
        }
    }
    else
    {
        if (TYPE(exp->R) == REAL_NUM)
        {
            if (DATA(exp->R).r == 1)
            {
                REPLACE_BRANCH(R, L)
            }
            if (DATA(exp->R).r == 0)
            {
                REPLACE_BRANCH(L, R) 
            }
        }
    }
    return 0;
}

int OneDiv (Node* node_exp)
{
    if (DATA(exp->R).r == 1)
    {
        REPLACE_BRANCH(R, L)
    }
}

#undef REPLACE_BRANCH