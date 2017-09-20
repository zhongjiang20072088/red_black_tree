//
// Created by zhongjiang on 17-5-18.
//
#include <limits.h>
#include <stdlib.h>
#ifndef REDBLACKTREE_RB_TREE_H
#define REDBLACKTREE_RB_TREE_H


enum COLOR {RED = 0, BLACK = 1};

typedef struct red_black_node {
    int key;
    enum COLOR color;
    struct red_black_node *parent;
    struct red_black_node *left;
    struct red_black_node *right;
}rb_node;

extern rb_node *NIL;

typedef struct red_black_node_list {
    rb_node *node;
    struct red_black_node_list *next;
}rb_list;

typedef struct red_black_tree {
    rb_list *head;
    rb_list *tail;
    rb_node *root;
    int count;
}rb_tree;

extern rb_tree* rb_tree_init();
extern int rb_tree_insert(rb_tree *tree, int key);
extern int rb_tree_delete(rb_tree *tree, int key);
extern int rb_tree_depth(rb_tree *tree);
extern void rb_tree_destroy(rb_tree *tree);
extern void rb_tree_dump(rb_tree *tree);

#endif //REDBLACKTREE_RB_TREE_H
