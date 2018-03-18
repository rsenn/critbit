#include "critbit.h"

#define EXTERNAL_NODE 0
#define INTERNAL_NODE 1

/* see http://cr.yp.to/critbit.html */
struct critbit_node {
  void * child[2];
  size_t byte;
  unsigned char mask;
};

int decode_pointer(void ** ptr);
void from_external_node(void * ptr, void **key, size_t *keylen);
void * cb_find_top_i(critbit_tree * cb, const void * key, size_t keylen);
