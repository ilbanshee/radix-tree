#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radix_tree.h"

int main() {
  node_t* root = node_init("aba");
  node_t* abab = node_init("b");
  node_t* b = node_init("b");
  node_t* bc = node_init("c");
  node_t* bac = node_init("ac");
  node_t* baca = node_init("a");

  root->link = abab;
  root->next = b;
  b->link = bc;
  bc->next = bac;
  bac->link = baca;

  node_t* tmp = node_insert(root, "bacobak", 0);

  tree_dump(root);

  node_t* p = node_find(root, "bacobak", 0);
  if (p != NULL) {
    printf("the node is: %s\n", p->key);
  } else {
    printf("NULL...\n");
  }

  node_remove(root, "baca", 0);

  p = node_find(root, "bacobak", 0);
  if (p != NULL) {
    printf("the node is: %s\n", p->key);
  } else {
    printf("NULL...\n");
  }

  // tree_free(root);

  node_free(root);
  node_free(abab);
  node_free(b);
  node_free(bc);
  node_free(bac);
  // node_free(baca);
  // node_free(tmp);

  return 0;
}
