#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "radix_tree.h"

int node_prefix(const char *x, int n, const char *key, int m);
void node_split(node_t *t, int k);
void node_join(node_t *t);
void tree_dump_with_level(node_t *root, int level);
void tree_dump(node_t *root);
node_t *node_insert_full(node_t *t, char *x, int n);
node_t *node_find_full(node_t *t, char *x, int n);
node_t *node_remove_full(node_t *t, char *x, int n);

node_t *node_init(const char *key) {
  int len = strlen(key);
  node_t *res = calloc(1, sizeof(node_t));
  res->key = calloc(len + 1, sizeof(char));
  strncpy(res->key, key, len);
  res->len = len;

  res->link = NULL;
  res->next = NULL;
  return res;
}

void node_free(node_t *node) {
  free(node->key);
  free(node);
}

/* length of the biggest common prefix of x and key strings */
int node_prefix(const char *x, int n, const char *key, int m) {
  for (int k = 0; k <= n; k++)
    if (k == m || x[k] != key[k]) return k;
  return n;
}

/* find node with given char* key, even a partial match is ok */
node_t *node_find(node_t *t, char *x) { return node_find_full(t, x, 0); }

/* find node with given char* key, an exact match is required */
node_t *node_find_exact(node_t *t, char *x) {
  node_t *res = node_find_full(t, x, 0);
  if (res == NULL) {
    return NULL;
  }
  size_t n = strlen(res->key);
  /* compare the last n chars of given char* to the result key that we found */
  if (strcmp(x + n, res->key) != 0) {
    return NULL;
  }

  return res;
}

node_t *node_find_full(node_t *t, char *x, int n) {
  if (!n) n = strlen(x);
  if (!t) return 0;
  int k = node_prefix(x, n, t->key, t->len);
  /* check child node */
  if (k == 0) {
    return node_find_full(t->next, x, n);
  }
  if (k == n) return t;
  /* check siblings node */
  if (k == t->len) {
    return node_find_full(t->link, x + k, n - k);
  }
  return NULL;
}

node_t *node_insert(node_t *t, char *x) { return node_insert_full(t, x, 0); }

node_t *node_insert_full(node_t *t, char *x, int n) {
  if (!n) n = strlen(x);
  if (!t) return node_init(x);
  int k = node_prefix(x, n, t->key, t->len);
  if (k == 0)
    t->next = node_insert_full(t->next, x, n);
  else if (k < n) {
    /* cut or not? */
    if (k < t->len) node_split(t, k);
    t->link = node_insert_full(t->link, x + k, n - k);
  }
  return t;
}

void node_split(node_t *t, int k) {
  node_t *p = node_init(t->key + k);
  p->link = t->link;
  t->link = p;
  char *a = calloc(k + 1, sizeof(char));
  strncpy(a, t->key, k);
  free(t->key);
  t->key = a;
  t->len = k;
}

/* merge t and t->link nodes */
void node_join(node_t *t) {
  node_t *p = t->link;
  char *a = calloc(t->len + p->len + 1, sizeof(char));
  strncpy(a, t->key, t->len);
  strncpy(a + t->len, p->key, p->len);
  free(t->key);
  t->key = a;
  t->len += p->len;
  t->link = p->link;
  node_free(p);
}

node_t *node_remove(node_t *t, char *x) { return node_remove_full(t, x, 0); }

node_t *node_remove_full(node_t *t, char *x, int n) {
  if (!n) n = strlen(x);
  if (!t) return 0;
  int k = node_prefix(x, n, t->key, t->len);
  /* deleting a leaf */
  if (k == n) {
    node_t *p = t->next;
    /* delete every linked node under this one */
    if (t->link) tree_free(t->link);
    node_free(t);
    return p;
  }
  if (k == 0) {
    t->next = node_remove_full(t->next, x, n);
  } else if (k == t->len) {
    t->link = node_remove_full(t->link, x + k, n - k);
    if (t->link && !t->link->next) node_join(t);
  }
  return t;
}

int tree_count(node_t *root) {
  int c = 1;
  node_t *tmp = root->link;
  if (tmp != NULL) {
    c += tree_count(tmp);
  }
  tmp = root->next;
  if (tmp != NULL) {
    c += tree_count(tmp);
  }
  return c;
}

int tree_count_entries(node_t *root) {
  int c = 0;
  if (root->link == NULL) {
    c = 1;
  }
  node_t *tmp = root->link;
  if (tmp != NULL) {
    c += tree_count_entries(tmp);
  }
  tmp = root->next;
  if (tmp != NULL) {
    c += tree_count_entries(tmp);
  }
  return c;
}

void tree_dump_full(node_t *root, char *prefix) {
  if (root->link == NULL) {
    printf("%s|%s\n", prefix, root->key);
  }

  node_t *tmp = root->link;
  if (tmp != NULL) {
    char *p = calloc(strlen(prefix) + strlen(root->key) + 1, sizeof(char));
    sprintf(p, "%s|%s", prefix, root->key);
    tree_dump_full(tmp, p);
    free(p);
  }

  tmp = root->next;
  if (tmp != NULL) {
    tree_dump_full(tmp, prefix);
  }
}

void tree_dump(node_t *root) { tree_dump_full(root, ""); }

void tree_free(node_t *root) {
  node_t *tmp = root->link;
  if (tmp != NULL) {
    tree_free(tmp);
  }
  tmp = root->next;
  if (tmp != NULL) {
    tree_free(tmp);
  }

  free(root->key);
  free(root);
}
