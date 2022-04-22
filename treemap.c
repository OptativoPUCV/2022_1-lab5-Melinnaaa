#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  tree->lower_than(key2,key1)==0) 
    {
      return 1; 
    }
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{   
    TreeMap* tmp = (TreeMap*) calloc (1, sizeof(TreeMap));
    if (tmp == NULL) return NULL;
    tmp->root = NULL;
    tmp->current = NULL;
    tmp->lower_than = lower_than;
    return tmp;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if (tree == NULL) return;

    if (searchTreeMap(tree, key) != NULL) return;
    TreeNode* tmp = tree->root;
    TreeNode* new = createTreeNode(key, value);

    while (tmp != NULL)
    {
        new->parent = tmp;
        if (tree->lower_than(key, tmp->pair->key) == 1)
        {
            tmp = tmp->left;
        }
        else if (tree->lower_than(key, tmp->pair->key) != 1)
        {
            tmp = tmp->right;
        }
    }
    if (new->parent == NULL)
    {
        tree->root = new;
    }
    else if (tree->lower_than(key, new->parent->pair->key) == 1)
    {
        new->parent->left = new;
    }
    else
    {
        new->parent->right = new;
    }
    tree->current = new;
}

TreeNode * minimum(TreeNode * x)
{
    if (x == NULL) return NULL;

    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, node->pair->key) == NULL) return;
    if (node->right == NULL && node->left != NULL)//Un hijo con rama izquierda no nula
    {
        if (tree->lower_than(node->parent->pair->key, node->left->pair->key) == 1)
        {
            node->parent->right = node->left;
        }
        else
        {
            node->parent->left = node->left;
        }
        free(node);
    }
    else if (node->right != NULL && node->left == NULL) //Un hijo con rama derecha no nula
    {
        if (tree->lower_than(node->parent->pair->key, node->right->pair->key) == 1)
        {
            node->parent->right = node->right;
        }
        else
        {
            node->parent->left = node->right;
        }
        free(node);
    }
    else if (node->right == NULL && node->left == NULL)
    {
        if(node->parent == NULL) // Caso en donde solo exista la raiz
        {
            tree->root=NULL; 
        }
        else if(node->parent->left == node) 
        {
            node->parent->left=NULL;
        }
        else if(node->parent->right==node) 
        {
            node->parent->right = NULL;
        }
        free(node);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* tmp = (TreeNode*) calloc (1, sizeof(TreeNode));
    tmp = tree->root;
    while (tmp != NULL)
    {
        if (is_equal(tree, key, tmp->pair->key))
        {
            tree->current = tmp;
            return tmp->pair;
        }
        else if (tree->lower_than(key, tmp->pair->key)  == 1)
        {
            tmp = tmp->left;
        }
        else if (tree->lower_than(key, tmp->pair->key) != 1)
        {
            tmp = tmp->right;
        }
    }

    tree->current = tmp;
    return NULL;
}



Pair * upperBound(TreeMap * tree, void* key) 
{
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* tmp = (TreeNode*) calloc (1, sizeof(TreeNode));
    TreeNode* ub = (TreeNode*) calloc (1, sizeof(TreeNode));
    tmp = tree->root;
    ub = NULL; 
    while (tmp != NULL)
    {
        if ((ub == NULL || tree->lower_than(tmp->pair->key, ub->pair->key) == 1) && (tree->lower_than(key, tmp->pair->key) == 1))    
        {
          ub = tmp;
        }
      
        if (is_equal(tree, key, tmp->pair->key))
        {
            return tmp->pair;
        }
        else if (tree->lower_than(key, tmp->pair->key)  == 1)
        {
            tmp = tmp->left;
        }
        else if (tree->lower_than(key, tmp->pair->key) != 1)
        {
            tmp = tmp->right;
        }
    }
    if (ub == NULL) return NULL;
    return ub->pair;
}


Pair * firstTreeMap(TreeMap * tree)
{
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* tmp = (TreeNode*) calloc (1, sizeof(TreeNode));
    tmp = tree->root;
    while (tmp->left != NULL)
    {
        tmp = tmp->left;
    }

    return tmp->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    if (tree == NULL || tree->root == NULL || tree->current == NULL) return NULL;

    TreeNode* tmp = (TreeNode*) calloc (1, sizeof(TreeNode));
    tmp = tree->current;
  
    if(tmp->right != NULL) //ඞmogus SUSSY BAKA 
    {
        tmp = tree->current->right;
        tree->current = minimum(tmp);
    }
    else if (tmp->right == NULL)
    {
        while (tree->lower_than(tmp->pair->key, tree->current->pair->key) == 1 || is_equal(tree,tree->current->pair->key, tmp->pair->key) == 1)
        {   
            if (tmp->parent!=NULL)
            {
              tmp = tmp->parent;
            }
            else 
            {
                return NULL;
            }
        }
        tree->current = tmp;
    }
    return tree->current->pair;
}