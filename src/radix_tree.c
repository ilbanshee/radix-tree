#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radix_tree.h"

int node_prefix(const char* x, int n, const char* key, int m);
void node_split(node_t* t, int k);
void node_join(node_t* t);
void tree_dump_with_level(node_t* root, int level);
void tree_dump(node_t* root);
node_t* node_insert(node_t* t, char* x, int n);

node_t* node_init(const char* key) {
  int len = strlen(key);
  node_t* res = calloc(1, sizeof(node_t));
  res->key = calloc(len + 1, sizeof(char));
  strncpy(res->key, key, len);
  res->len = len;

  res->link = NULL;
  res->next = NULL;
  return res;
}

void node_free(node_t* node) {
  free(node->key);
  free(node);
}

// length of the biggest common prefix of x and key strings
int node_prefix(const char* x, int n, const char* key, int m) {
  for (int k = 0; k <= n; k++)
    if (k == m || x[k] != key[k]) return k;
  return n;
}

node_t* node_find(node_t* t, char* x, int n) {
  // printf("node: %s, x: %s, n: %d\n", t->key, x, n);
  if (!n) n = strlen(x);
  if (!t) return 0;
  int k = node_prefix(x, n, t->key, t->len);
  // printf("k: %d n: %d\n", k, n);
  // check child node
  if (k == 0) {
    return node_find(t->next, x, n);
  }
  if (k == n) return t;
  // check siblings node
  if (k == t->len) {
    return node_find(t->link, x + k, n - k);
  }
  return NULL;
}

node_t* node_insert(node_t* t, char* x, int n) {
  if (!n) n = strlen(x);
  if (!t) return node_init(x);
  int k = node_prefix(x, n, t->key, t->len);
  if (k == 0)
    t->next = node_insert(t->next, x, n);
  else if (k < n) {
    // cut or not?
    if (k < t->len) node_split(t, k);
    t->link = node_insert(t->link, x + k, n - k);
  }
  return t;
}

void node_split(node_t* t, int k) {
  node_t* p = node_init(t->key + k);
  p->link = t->link;
  t->link = p;
  char* a = calloc(k + 1, sizeof(char));
  strncpy(a, t->key, k);
  free(t->key);
  t->key = a;
  t->len = k;
}

// merge t and t->link nodes
void node_join(node_t* t) {
  node_t* p = t->link;
  char* a = calloc(t->len + p->len + 1, sizeof(char));
  strncpy(a, t->key, t->len);
  strncpy(a + t->len, p->key, p->len);
  free(t->key);
  t->key = a;
  t->len += p->len;
  t->link = p->link;
  node_free(p);
}

node_t* node_remove(node_t* t, char* x, int n) {
  if (!n) n = strlen(x);
  if (!t) return 0;
  int k = node_prefix(x, n, t->key, t->len);
  // deleting a leaf
  if (k == n) {
    node_t* p = t->next;
    node_free(t);
    return p;
  }
  if (k == 0) {
    t->next = node_remove(t->next, x, n);
  } else if (k == t->len) {
    t->link = node_remove(t->link, x + k, n - k);
    if (t->link && !t->link->next) node_join(t);
  }
  return t;
}

void tree_dump_with_level(node_t* root, int level) {
  for (int i = 0; i < level; i++) {
    printf("-");
  }
  printf("> %s\n", root->key);

  node_t* tmp = root->next;
  while (tmp) {
    tree_dump_with_level(tmp, level + 1);
    tmp = tmp->next;
  }

  tmp = root->link;
  while (tmp) {
    tree_dump_with_level(tmp, level);
    tmp = tmp->link;
  }
}

void tree_dump(node_t* root) { tree_dump_with_level(root, 0); }
