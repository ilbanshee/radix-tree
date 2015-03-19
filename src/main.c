#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "radix_tree.h"

#define NAMES "test-data/name-list.txt"
#define DNS "test-data/random-domains.txt"

void t_with_insert() {
  node_t* root = node_init("com.google");
  node_insert(root, "com.amazon", 0);
  node_insert(root, "it.amazon", 0);
  node_insert(root, "it.google", 0);
  node_insert(root, "com.amazing-spiderman", 0);
  node_insert(root, "com.ebay.deals", 0);
  node_insert(root, "com.ebay.signin", 0);
  node_insert(root, "com.oracle.java", 0);
  node_insert(root, "com.oracle.db", 0);
  assert(tree_count(root) == 14);
  node_t* node = node_find(root, "it.", 0);
  assert(node != NULL);
  assert(tree_count(node) == 3);
  node = node_find(root, "com.ebay.", 0);
  assert(node != NULL);
  assert(tree_count(node) == 6);

  assert(14 == tree_count(root));
  assert(9 == tree_count_entries(root));

  tree_free(root);
}

void t_by_hand() {
  node_t* root1 = node_init("com.");
  node_t* com_amazon = node_init("amazon");
  node_t* com_ebay = node_init("ebay.");
  node_t* com_oracle = node_init("oracle.");
  node_t* com_ebay_deals = node_init("deals");
  node_t* com_ebay_signin = node_init("signin");
  node_t* com_oracle_java = node_init("java");
  node_t* com_oracle_db = node_init("db");
  node_t* it = node_init("it.");
  node_t* it_oracle = node_init("oracle.");
  node_t* it_oracle_db = node_init("db");
  node_t* it_google = node_init("google");
  node_t* it_amazon = node_init("amazon");
  node_t* it_alice = node_init("alice");

  root1->link = com_ebay;
  com_ebay->link = com_ebay_deals;
  com_ebay_deals->next = com_ebay_signin;

  assert(4 == tree_count(root1));

  com_ebay->next = com_oracle;
  com_oracle->link = com_oracle_java;
  com_oracle->next = com_amazon;
  com_oracle_java->next = com_oracle_db;

  root1->next = it;
  it->link = it_oracle;
  it_oracle->next = it_google;
  it_oracle->link = it_oracle_db;
  it_google->next = it_amazon;
  it_amazon->next = it_alice;

  assert(9 == tree_count_entries(root1));
  tree_free(root1);
}

void t_long_list() {
  node_t* names = node_init("");
  FILE* file = fopen(DNS, "r");
  if (file != NULL) {
    char line[128];
    while (fgets(line, sizeof line, file) != NULL) {
      node_insert(names, line, 0);
    }
    fclose(file);
  } else {
    perror(NAMES);
  }
  assert(9794 + 1 == tree_count_entries(names));
  tree_free(names);

  names = node_init("");
  file = fopen(NAMES, "r");
  if (file != NULL) {
    char line[128];
    while (fgets(line, sizeof line, file) != NULL) {
      node_insert(names, line, 0);
    }
    fclose(file);
  } else {
    perror(NAMES);
  }
  assert(200 + 1 == tree_count_entries(names));
  tree_free(names);
}

int main() {
  t_with_insert();
  t_by_hand();
  t_long_list();

  // TODO write some tests for find and delete...

  /*
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
  */

  return 0;
}
