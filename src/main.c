#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "radix_tree.h"

#define NAMES "test-data/name-list.txt"
#define DNS "test-data/random-domains.txt"
#define CENSUS "test-data/census.txt"

void t_with_insert() {
  printf("Testing manual node insert ops                      ");
  node_t* root = node_init("com.google");
  node_insert(root, "com.amazon");
  node_insert(root, "it.amazon");
  node_insert(root, "it.google");
  node_insert(root, "com.amazing-spiderman");
  node_insert(root, "com.ebay.deals");
  node_insert(root, "com.ebay.signin");
  node_insert(root, "com.oracle.java");
  node_insert(root, "com.oracle.db");
  assert(tree_count(root) == 14);
  node_t* node = node_find(root, "it.");
  assert(node != NULL);
  assert(tree_count(node) == 3);
  node = node_find(root, "com.ebay.");
  assert(node != NULL);
  assert(tree_count(node) == 6);
  assert(14 == tree_count(root));
  assert(9 == tree_count_entries(root));

  tree_free(root);
  printf("[OK]\n");
}

void t_by_hand() {
  printf("Testing insert function                             ");
  node_t* root = node_init("com.");
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

  root->link = com_ebay;
  com_ebay->link = com_ebay_deals;
  com_ebay_deals->next = com_ebay_signin;

  assert(4 == tree_count(root));

  com_ebay->next = com_oracle;
  com_oracle->link = com_oracle_java;
  com_oracle->next = com_amazon;
  com_oracle_java->next = com_oracle_db;

  root->next = it;
  it->link = it_oracle;
  it_oracle->next = it_google;
  it_oracle->link = it_oracle_db;
  it_google->next = it_amazon;
  it_amazon->next = it_alice;

  assert(9 == tree_count_entries(root));
  tree_free(root);
  printf("[OK]\n");
}

void t_name_list() {
  printf("Loading simple name list                            ");
  node_t* names;
  bool first = true;
  FILE* file = fopen(NAMES, "r");
  if (file != NULL) {
    char line[128];
    while (fgets(line, sizeof line, file) != NULL) {
      if (first) {
        names = node_init(line);
        first = false;
      } else {
        node_insert(names, line);
      }
    }
    fclose(file);
  } else {
    perror(NAMES);
  }
  assert(200 == tree_count_entries(names));
  tree_free(names);
  printf("[OK]\n");
}

void t_partial_domain_list() {
  printf("Loading 10k string list                             ");
  node_t* names;
  bool first = true;
  FILE* file = fopen(DNS, "r");
  if (file != NULL) {
    char line[128];
    while (fgets(line, sizeof line, file) != NULL) {
      if (first) {
        names = node_init(line);
        first = false;
      } else {
        node_insert(names, line);
      }
    }
    fclose(file);
  } else {
    perror(DNS);
  }
  assert(9794 == tree_count_entries(names));
  tree_free(names);
  printf("[OK]\n");
}

void t_census_list() {
  printf("Loading 90k name list                               ");
  node_t* names;
  bool first = true;
  FILE* file = fopen(CENSUS, "r");
  if (file != NULL) {
    char line[128];
    while (fgets(line, sizeof line, file) != NULL) {
      if (first) {
        names = node_init(line);
        first = false;
      } else {
        node_insert(names, line);
      }
    }
    fclose(file);
  } else {
    perror(CENSUS);
  }
  assert(88799 == tree_count_entries(names));
  tree_free(names);
  printf("[OK]\n");
}

void t_remove() {
  printf("Testing remove operation                            ");
  node_t* names;
  bool first = true;
  FILE* file = fopen(NAMES, "r");
  if (file != NULL) {
    char line[128];
    while (fgets(line, sizeof line, file) != NULL) {
      char* pos;
      if ((pos = strchr(line, '\n')) != NULL) *pos = '\0';

      if (first) {
        names = node_init(line);
        first = false;
      } else {
        node_insert(names, line);
      }
    }
    fclose(file);
  } else {
    perror(NAMES);
  }
  assert(200 == tree_count_entries(names));

  names = node_remove(names, "Leonore Krach");
  assert(199 == tree_count_entries(names));

  names = node_remove(names, "Hugo Penfield");
  assert(198 == tree_count_entries(names));

  names = node_remove(names, "Hugo Penfield");
  names = node_remove(names, "Leonore Krach");
  assert(198 == tree_count_entries(names));

  tree_free(names);
  printf("[OK]\n");
}

void t_find() {
  printf("Testing find and partial find operations            ");
  node_t* names;
  bool first = true;
  FILE* file = fopen(NAMES, "r");
  if (file != NULL) {
    char line[128];
    while (fgets(line, sizeof line, file) != NULL) {
      char* pos;
      if ((pos = strchr(line, '\n')) != NULL) *pos = '\0';

      if (first) {
        names = node_init(line);
        first = false;
      } else {
        node_insert(names, line);
      }
    }
    fclose(file);
  } else {
    perror(NAMES);
  }
  assert(200 == tree_count_entries(names));

  node_t* tmp = node_find(names, "Leonore Kra");
  assert(tmp != NULL);
  assert(strcmp(tmp->key, "ore Krach") == 0);
  tmp = node_find(names, "eonore Krach");
  assert(tmp == NULL);
  tmp = node_find_exact(names, "Leonore Kra");
  assert(tmp == NULL);
  tmp = node_find_exact(names, "szvxcgdfgsdf");
  assert(tmp == NULL);
  tmp = node_find(names, "Leonore Krach");
  assert(tmp != NULL);
  assert(strcmp(tmp->key, "ore Krach") == 0);

  tree_free(names);
  printf("[OK]\n");
}

int main() {
  /* test tree creation */
  t_with_insert();
  t_by_hand();
  t_name_list();
  t_census_list();
  t_partial_domain_list();
  /* test elements removal */
  t_remove();
  /* test find functions */
  t_find();

  return 0;
}
