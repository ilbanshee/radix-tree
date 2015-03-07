#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radix_tree.h"

int main() {
  node_t* root = node_init("com.google");
  node_insert(root, "com.amazon", 0);
  node_insert(root, "it.amazon", 0);
  node_insert(root, "it.google", 0);
  node_insert(root, "com.amazing-spiderman", 0);
  node_insert(root, "com.ebay.deals", 0);
  node_insert(root, "com.ebay.signin", 0);
  node_insert(root, "com.oracle.java", 0);
  node_insert(root, "com.oracle.db", 0);

  tree_dump(root);

  node_t* p = node_find(root, "com.amazing-spiderman", 0);
  if (p != NULL) {
    printf("the node is: %s\n", p->key);
  } else {
    printf("NULL...\n");
  }

  p = node_find(root, "com.oracle.java", 0);
  if (p != NULL) {
    printf("the node is: %s\n", p->key);
  } else {
    printf("NULL...\n");
  }

  return 0;
}
