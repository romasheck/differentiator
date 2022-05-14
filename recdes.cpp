#include "recdes.h"

char* TakeTask (FILE* task)
{
    struct stat size;
    fstat(fileno(task), &size);

    char* buffer = (char*) calloc(size.st_size + 2, sizeof(char));

    fread (buffer, size.st_size, sizeof(char), task);

    return buffer;
}

int GetG (const char* buf, Node* head)
{
    uint32_t pos = 0;

    bool flag_ncE = 0;
    GetE (buf, &pos, head, &flag_ncE);

    if (buf[pos] == '$')
    {
        return CORRECT;
    }
    else
    {
        return INCORRECT;
    }
}

#undef CUR_S
#define CUR_S buf[*pos]

int GetE (const char* buf, uint32_t* pos, Node* node, bool* flag_ncE)
{
    NodeInsert (node, {NOT_A_TYPE, 0}, LEFT_N);
    bool flag_ncT = 0;
    GetT (buf, pos, LNODE, &flag_ncT);

    if (CUR_S == '+')
    {
        *pos += 1;

        TYPE = BIN_OPERATOR;
        DATA.c  = '+';

        NodeInsert (node, {NOT_A_TYPE, 0}, RIGHT_N);
        GetE (buf,  pos, RNODE, flag_ncE);

        return 0;
    }
    if (CUR_S == '-')
    {
        *pos += 1;
        
        TYPE = BIN_OPERATOR;
        DATA.c  = ((*flag_ncE) ? '+' : '-');
        *flag_ncE = 1;

        NodeInsert (node, {NOT_A_TYPE, 0}, RIGHT_N);
        GetE (buf,  pos, RNODE, flag_ncE);

        return 0;
    }

    *node = *(LNODE);
    
    return 0;
}

int GetT (const char* buf, uint32_t* pos, Node* node, bool* flag_ncT)
{
    NodeInsert (node, {NOT_A_TYPE, 0}, LEFT_N);
    GetD (buf, pos, LNODE);

    if (CUR_S == '*')
    {
        *pos += 1;

        TYPE = BIN_OPERATOR;
        DATA.c  = '*';

        NodeInsert (node, {NOT_A_TYPE, 0}, RIGHT_N);
        GetT (buf,  pos, RNODE, flag_ncT);

        return 0;
    }
    if (CUR_S == '/')
    {
        *pos += 1;

        TYPE = BIN_OPERATOR;
        DATA.c  = ((*flag_ncT) ? '*' : '/');
        *flag_ncT = 1;

        NodeInsert (node, {NOT_A_TYPE, 0}, RIGHT_N);
        GetT (buf,  pos, RNODE, flag_ncT);

        return 0;
    }

    *node = *(LNODE);

    return 0;
}

int GetD (const char* buf, uint32_t* pos, Node* node)
{
    NodeInsert (node, {NOT_A_TYPE, 0}, LEFT_N);
    GetP (buf, pos, LNODE);

    if (CUR_S == '^')
    {
        *pos += 1;

        TYPE = BIN_OPERATOR;
        DATA.c  = '^';

        NodeInsert (node, {NOT_A_TYPE, 0}, RIGHT_N);
        GetD (buf,  pos, RNODE);

        return 0;
    }

    *node = *(LNODE);

    return 0;
}

int GetP (const char* buf, uint32_t* pos, Node* node)
{
    if (CUR_S == '(')
    {
        *pos += 1;
        bool flag_ncE = 0;
        GetE (buf, pos, node, &flag_ncE);

        if (CUR_S == ')')
        {
            *pos += 1;
            return 0;
        }
        else 
        {
            printf ("lexical error in func %s with cur_s %c\n", __func__, CUR_S);
            return 1;
        }
    }
    else 
    {
        GetU (buf, pos, node);
        return 0;
    }
}

int GetU (const char* buf, uint32_t* pos, Node* node)
{
    if ('0' <= CUR_S && CUR_S <= '9')//NUMBER
    {
        GetN (buf, pos, node);
        return 0;
    }

    if (strncmp(buf + *pos, "ln", 2) == 0)//[uop]P ln not used, because we are haven't const^x
    {
        DATA.c = 'l';
        TYPE = UNO_OPERATOR;
        *pos += 2;
        NodeInsert (node, {NOT_A_TYPE, 0}, LEFT_N);
        GetP (buf, pos, LNODE);
        return 0;
    }
    if (strncmp(buf + *pos, "sin", 3) == 0)
    {
        DATA.c = 's';
        TYPE = UNO_OPERATOR;
        *pos += 3;
        NodeInsert (node, {NOT_A_TYPE, 0}, LEFT_N);
        GetP (buf, pos, LNODE);
        return 0;
    }
    if (strncmp(buf + *pos, "cos", 3) == 0)
    {
        DATA.c = 'c';
        TYPE = UNO_OPERATOR;
        *pos += 3;
        NodeInsert (node, {NOT_A_TYPE, 0}, LEFT_N);
        GetP (buf, pos, LNODE);
        return 0;
    }

    if ('a' <= CUR_S && CUR_S <= 'z')//VARIABLE
    {
        GetV (buf, pos, node);
        return 0;
    }
    
    printf ("lexical error in func %s with cur_s = %c\n", __func__, CUR_S);
    return 1;
}

int GetV (const char* buf, uint32_t* pos, Node* node)
{
    if ('a' <= CUR_S && CUR_S <= 'z')
    {
        sscanf(buf + *pos, "%c", &DATA.c);
        TYPE = VARIABLE;
        //printf ("%c\n", DATA.c);//db
        *pos += 1;
        return 0;
    }
    else
    {
        printf ("lexical error in func %s with cur_s = %c\n", __func__, CUR_S);
        return 1;
    }
}

int GetN (const char* buf, uint32_t* pos, Node* node)
{
    uint32_t prev_pos = *pos;

    while ('0' <= CUR_S && CUR_S <= '9' || CUR_S == '.')
    {
        *pos += 1;  
    }

    if (*pos - prev_pos)
    {
        sscanf (buf + prev_pos, "%lf", &DATA.r);
        TYPE = REAL_NUM;
        return 0;
    }
    else 
    {
        printf ("lexical error in func %s with cur_s = %c\n", __func__, CUR_S);
        return 1;
    }
}


#undef CUR_S