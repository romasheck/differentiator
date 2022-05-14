#include "differentiator.h"

int main ()
{
    FILE* task = fopen ("task.txt", "r");

    Tree* tree_task = TakeTree (task);
    fclose (task);
    
    Tree asscar_lox;
    Tree* tree_answer = &asscar_lox;
    TreeCtor (tree_answer);

    GoDif (tree_task, tree_answer);
    //CopyNodes (THEAD(task), THEAD(answer));
    
    TreeDump (tree_answer, "log_tree_answer.txt", "TreeLogAnswer.png");

    TreeDtor (tree_answer);
    TreeDtor (tree_task);

    return 0;
}