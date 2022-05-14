#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <iostream>
#include <sys\stat.h>
#include <cstdio>
#include <cassert>
#include <string.h>

enum TSIDES
{
    RIGHT_N = 0,
    LEFT_N = 1
};

enum DATA_TYPES
{
    NOT_A_TYPE,
    INTEGER,
    REAL_NUM,
    BIN_OPERATOR,
    UNO_OPERATOR,
    VARIABLE,
    STR,
    OTHER
};

enum FREE_IN_DTOR
{
    NEED_FREE,
    NOT_NEED_FREE
};

union NodeDataType
{
    char    c;
    int     i;
    double  r;
    char*   cp;
};

struct node_data_t
{
    char type;
    NodeDataType data;
};

struct Node
{
    node_data_t n_data;
    char free_flag;
    Node* left_node;
    Node* right_node;
};

#define DATA node->n_data.data
#define TYPE node->n_data.type
#define RNODE node->right_node
#define LNODE node->left_node

struct Tree 
{
    Node* head;
    size_t size;
};

#define THEAD (tree->head)

int                 TreeCtor                (Tree* tree);

//node_data_t         DataMake                (char type, NodeDataType data);

int                 NodeInsert              (Node* node, node_data_t data, int side);

int                 TreeDtor                (Tree* tree);

int                 NodesDtor               (Node* node);

int                 TreeDump                (Tree* tree, const char* log_File, const char* png_File);

int                 NodesIntroduceDump      (Node* node, FILE* log_file);

int                 Node_IntroduceDump      (Node* node, FILE* log_file);

int                 NodesConnectDump        (Node* node, FILE* log_file);
/*
int                 Node_ConnectDump        (Node* node, FILE* log_file);
*/
int                 TreeSave                (Tree* tree, FILE* tree_file);

int                 NodesWrite              (Node* node, FILE* tree_file);

int                 Node_Write              (Node* node, FILE* tree_file);

char*               TreeTake                (Tree* tree, FILE* tree_file);

int                 NodesRead               (Node* node, Node* head, uint32_t* buf_ptr);

int                 PutDataInNode           (Node* node, uint32_t* buf_ptr);
                               
#define PRINT_LINE printf("I am at the line %d\n", __LINE__);
#define MAKE_DATA(data_name, code_type, short_type, dt)    \
data_name.type = code_type;                                \
data_name.data.short_type = dt;                            

#endif