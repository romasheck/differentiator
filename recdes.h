#ifndef RECDES_H_INCLUDED
#define RECDES_H_INCLUDED

#include <iostream>
#include <sys\stat.h>
#include "tree.h"

enum LexicalCheck
{
    CORRECT   = 0, 
    INCORRECT = 1
};

char*           TakeTask            (FILE* task);

int             GetG                (const char* buf, Node* head);

int             GetE                (const char* buf, uint32_t* pos, Node* node, bool* flag_ncE);

int             GetT                (const char* buf, uint32_t* pos, Node* node, bool* flag_ncT);

int             GetD                (const char* buf, uint32_t* pos, Node* node);

int             GetP                (const char* buf, uint32_t* pos, Node* node);

int             GetU                (const char* buf, uint32_t* pos, Node* node);

int             GetV                (const char* buf, uint32_t* pos, Node* node);

int             GetN                (const char* buf, uint32_t* pos, Node* node);

#endif