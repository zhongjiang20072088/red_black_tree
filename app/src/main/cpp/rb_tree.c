//
// Created by zhongjiang on 17-5-18.
//

#include "rb_tree.h"


#include <stdio.h>
#include <android/log.h>
#include <math.h>

static rb_node nil = {INT_MAX, BLACK, NULL, NULL, NULL};

rb_node *NIL = &nil;

rb_tree *rb_tree_init() {
    rb_tree* tree = (rb_tree *)malloc(sizeof(rb_tree));
    if (tree != NULL) {
        tree->root = NIL;
        tree->head = NULL;
        tree->tail = NULL;
        tree->count = 0;
    }
    return tree;
}

void rb_tree_destroy(rb_tree *tree) {
    rb_list *list = tree->head;
    while (list != NULL) {
        rb_list *next = list->next;
        //__android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "free %d\n", list->node->key);
        free(list->node);
        free(list);
        list = next;
    }
    free(tree);
}

static void dump(rb_node *node) {
    rb_list *list = (rb_list *)malloc(sizeof(rb_list));
    list->node = node;
    list->next = NULL;
    rb_list *list1 = NULL;
    while(list != NULL) {
        __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "key = %d", list->node->key);
        if (list->node->left != NIL) {
            rb_list *l = (rb_list *)malloc(sizeof(rb_list));
            l->node = list->node->left;
            l->next = list1;
            list1 = l;
        }
        if (list->node->right != NIL) {
            rb_list *l = (rb_list *)malloc(sizeof(rb_list));
            l->node = list->node->right;
            l->next = list1;
            list1 = l;
        }
        list = list->next;
        if (list == NULL) {
            list = list1;
            list1 = NULL;
            __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "\n");
        }
    }
}

void rb_tree_dump(rb_tree *tree) {
    if (tree->root != NIL)
        dump(tree->root);
    /*rb_list *list = tree->head;
    while (list != NULL) {
        rb_list *next = list->next;
        *//*__android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "node = %d x = %d y = %d\n", list->node->key, list->node->x, list->node->y);
        if (list->node->parent != NULL) {
            __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "parent = %d x = %d y = %d\n", list->node->parent->key, list->node->parent->x, list->node->parent->y);
        }*//*
        __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "node = %d", list->node->key);
        if (list->node->color == RED)
            __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " red\n");
        else
            __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " black\n");
        __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " ------------------------\n");
        if (list->node->parent != NULL) {
            __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "parent = %d", list->node->parent->key);
            if (list->node->parent->color == RED)
                __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " red\n");
            else
                __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " black\n");
            __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " ------------------------\n");
        }
        list = next;
    }*/
}



static rb_node *sibling(rb_node *node) {
    if (node->parent == NULL) {
        return NULL;
    } else {
        if (node->parent->left == node)
            return node->parent->right;
        else
            return node->parent->left;
    }
}

static rb_node *uncle(rb_node *node) {
    if (node->parent != NULL && node->parent->parent != NULL) {
        if (node->parent->parent->left == node->parent)
            return node->parent->parent->right;
        else
            return node->parent->parent->left;
    }
    return NULL;
}


// tree is the root of the red black tree, node is to rotate & node->right != NULL
static void left_rotate(rb_tree *tree, rb_node *node) {
    rb_node *right = node->right;
    node->right = right->left;
    right->left->parent = node;
    right->parent = node->parent;
    if (right->parent == NULL) {
        tree->root = right;
    } else if (right->parent->left == node) {
        right->parent->left = right;
    } else {
        right->parent->right = right;
    }
    right->left = node;
    node->parent = right;
}

// tree is the root of the red black tree, node is to rotate & node->left != NULL
static void right_rotate(rb_tree *tree, rb_node *node) {
    rb_node *left = node->left;
    node->left = left->right;
    left->right->parent = node;
    left->parent = node->parent;
    if (left->parent == NULL) {
        tree->root = left;
    } else if (left->parent->left == node) {
        left->parent->left = left;
    } else {
        left->parent->right = left;
    }
    left->right = node;
    node->parent = left;
}

static void rb_tree_insert_fix_up(rb_tree *tree, rb_node *node) {
    while(node->parent->color == RED) {
        rb_node *u = uncle(node);
        if(node->parent->parent->left == node->parent) {
            if(u->color == RED) {
                u->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if(node->parent->right == node) {
                    node->parent->color = BLACK;
                    left_rotate(tree, node->parent);
                    right_rotate(tree, node->parent);
                } else {
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    right_rotate(tree, node->parent->parent);
                }
            }
        } else {
            if(u->color == RED) {
                u->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if(node->parent->right == node) {
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    left_rotate(tree, node->parent->parent);
                } else {
                    //node = node->parent;
                    node->parent->color = BLACK;
                    right_rotate(tree, node->parent);
                    left_rotate(tree, node->parent);
                }
            }
        }
        if (node->parent == NULL) {
            node->color = BLACK;
            break;
        }
    }
    tree->root->color = BLACK;
}

/**
 * tree, the red black tree that will insert a node if there's no node which key = key
 * return 0 success, -1 fail
 */
int rb_tree_insert(rb_tree *tree, int key) {
    rb_node *pre = NULL;
    rb_node *current = tree->root;
    __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "insert key %d", key);
    while (current != NIL) {
        pre = current;
        if (key < current->key)
            current = current->left;
        else if (key > current->key)
            current = current->right;
        else
            return -1;
    }
    //__android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", "insert %d\n", key);
    rb_node *node = (rb_node *)malloc(sizeof(rb_node));
    node->key = key;

    rb_list *list = (rb_list *)malloc(sizeof(rb_list));
    list->node = node;
    list->next = NULL;
    if (tree->head == NULL) {
        tree->head = list;
        tree->tail = list;
    } else {
        tree->tail->next = list;
        tree->tail = list;
    }

    if (pre == NULL) {
        tree->root = node;
        node->parent = NULL;
    } else {
        if(key < pre->key) {
            pre->left = node;
        } else {
            pre->right = node;
        }
        node->parent = pre;
    }
    node->left = NIL;
    node->right = NIL;
    if(tree->root == node)
        node->color = BLACK;
    else {
        node->color = RED;
        rb_tree_insert_fix_up(tree, node);
    }
    tree->count++;
    return 0;
}

static void rb_tree_delete_fix_up(rb_tree *tree, rb_node *node) {
    while (node->parent != NULL) {
        __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " rb_tree_delete 3");
        rb_node *s = sibling(node);
        if (s->color == RED) { //sibling children's color = black, parent's color = black
            s->color = BLACK;
            node->parent->color = RED;
            if (node->parent->left == node)
                left_rotate(tree, node->parent);
            else
                right_rotate(tree, node->parent);
            s = sibling(node);
        }
        if (node->parent->color == BLACK && s->color == BLACK
            && s->left->color == BLACK && s->right->color == BLACK) {
            s->color = RED;
            node = node->parent;
            continue;
        }
        if (node->parent->color == RED && s->color == BLACK
            && s->left->color == BLACK && s->right->color == BLACK) {
            //node->parent->color = BLACK;
            if (node->parent->left == node)
                left_rotate(tree, node->parent);
            else
                right_rotate(tree, node->parent);
            break;
        }
        if (node->parent->left == node && s->color == BLACK
            && s->left->color == RED && s->right->color == BLACK) {
            s->color = RED;
            s->left->color = BLACK;
            right_rotate(tree, s);
            s = sibling(node);
        } else if (node->parent->right == node && s->color == BLACK
                   && s->left->color == BLACK && s->right->color == RED) {
            s->color = RED;
            s->right->color = BLACK;
            left_rotate(tree, s);
            s = sibling(node);
        }
        if (node->parent->left == node && s->color == BLACK
            /*&& (s->left->color == BLACK)*/&& s->right->color == RED) {
            s->color = node->parent->color;
            node->parent->color = BLACK;
            s->right->color = BLACK;
            left_rotate(tree, node->parent);
            break;
        } else if (node->parent->right == node && s->color == BLACK
                   && s->left->color == RED /*&& s->right->color == BLACK*/) {
            s->color = node->parent->color;
            node->parent->color = BLACK;
            s->left->color = BLACK;
            right_rotate(tree, node->parent);
            break;
        }
    }
}

/**
 * delete a node if there is a node which key = key
 * return 0 success, -1 fail
 */
int rb_tree_delete(rb_tree *tree, int key) {
    rb_node *current = tree->root;
    while(current != NIL) {
        __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " rb_tree_delete");
        if(current->key == key)
            break;
        else if(current->key < key)
            current = current->right;
        else
            current = current->left;
    }
    if (current != NIL) {
        if (current->left == NIL || current->right == NIL) {
            rb_node *parent = current->parent;
            if (parent == NULL) {
                tree->root = current->left != NIL ? current->left : current->right;
                tree->root->color = BLACK;
                tree->root->parent = NULL;
            } else {
                if (current->left == NIL && current->right == NIL) {
                    if (current->color == RED) {
                        if (parent->left == current)
                            parent->left = NIL;
                        else
                            parent->right = NIL;
                    } else {
                        rb_tree_delete_fix_up(tree, current);
                        if (current->parent->left == current)
                            current->parent->left = NIL;
                        else
                            current->parent->right = NIL;
                    }
                } else {//in this case current's color = BLACK and it's only child's color = RED
                    rb_node *child = current->left != NIL ? current->left : current->right;
                    if (parent->left == current) {
                        parent->left = child;
                    } else {
                        parent->right = child;
                    }
                    child->parent = parent;
                    child->color = BLACK;
                    //rb_tree_delete_fix_up(tree, child);
                }
            }
        } else {
            rb_node *suc = current->left;
            __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " rb_tree_delete 2");
            while (suc->right != NIL)
                suc = suc->right;
            current->key = suc->key;
            if (suc->left != NIL) {//suc->left->color = RED;
                if (suc->parent->left == suc) {
                    suc->parent->left = suc->left;
                } else {
                    suc->parent->right = suc->left;
                }
                suc->left->parent = suc->parent;
                suc->left->color = BLACK;
                //rb_tree_delete_fix_up(tree, suc->left);
            } else {
                if (suc->color == RED) {
                    if (suc->parent->left == suc)
                        suc->parent->left = NIL;
                    else
                        suc->parent->right = NIL;
                } else {
                    rb_tree_delete_fix_up(tree, suc);
                    if (suc->parent->left == suc)
                        suc->parent->left = NIL;
                    else
                        suc->parent->right = NIL;
                }
            }
            current = suc;
        }
        tree->count--;
        rb_list *pre = NULL;
        rb_list *cur = tree->head;
        while (/*cur != NULL && */cur->node != current) {
            pre = cur;
            cur = cur->next;
            __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " rb_tree_delete 1");
        }

        if (cur == tree->head) //cur != NULL;
            tree->head = cur->next;
        if (cur == tree->tail)
            tree->tail = pre;
        if (pre != NULL)
            pre->next = cur->next;
        free(cur);
        free(current);
        return 0;
    } else {
        return -1;
    }
}

static int max(int a, int b) {
    return a > b ? a : b;
}

static int __rb_tree_depth_inner(rb_node *node) {
    if (node == NIL)
        return 0;
    return max(__rb_tree_depth_inner(node->left), __rb_tree_depth_inner(node->right)) + 1;

}

int rb_tree_depth(rb_tree *tree) {
    __android_log_print(ANDROID_LOG_DEBUG, "zhongjiang", " count  %d", tree->count);
    return __rb_tree_depth_inner(tree->root);
}