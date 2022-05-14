#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include "recdes.h"

Tree*       TakeTree        (FILE* task);

int         GoDif           (Tree* tree_task, Tree* tree_answer);

int         DifNodes        (Node* node_task, Node* node_answer);

int         DifLeaf         (Node* node_task, Node* node_answer);

int         DifBOp          (Node* node_task, Node* node_answer);

int         DifUOp          (Node* node_task, Node* node_answer);

int         CopyNodes       (Node* node_task, Node* node_answer);


//undef our DSL:
#undef LNODE
#undef RNODE
#undef TYPE
#undef DATA
#undef THEAD
#undef L
#undef R
#undef T
#undef D
#undef NODES
#undef LNODES
#undef RNODES
//DSL:
#define LNODE(name) node_##name->left_node
#define RNODE(name) node_##name->right_node
#define TYPE(name)  node_##name->n_data.type
#define DATA(name)  node_##name->n_data.data
#define THEAD(name) tree_##name->head
#define L           left_node
#define R           right_node
#define T           n_data.type
#define D           n_data.data
#define NODES       node_task, node_answer
#define LNODES      LNODE(task), LNODE(answer)
#define RNODES      RNODE(task), RNODE(answer)
//

#endif