#ifndef RADIX_TREE_H_
#define RADIX_TREE_H_

typedef struct node {  // a structure to represent tree nodes
  char* key;
  int len;
  struct node* link;
  struct node* next;
} node_t;

node_t* node_find(node_t* t, char* x, int n);
node_t* node_insert(node_t* t, char* x, int n);
node_t* node_remove(node_t* t, char* x, int n);

node_t* node_init(const char* key);
void node_free(node_t* node);
void tree_dump(node_t* root);
int tree_count(node_t* root);
void tree_free(node_t* root);
int tree_count_entries(node_t* root);

#endif  // RADIX_TREE_H_
