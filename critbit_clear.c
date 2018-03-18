#include "critbit_internal.h"
#include <stdlib.h>

static void
critbit_free_node(void * ptr) {
  if(decode_pointer(&ptr) == INTERNAL_NODE) {
    struct critbit_node * node = (struct critbit_node *)ptr;
    critbit_free_node(node->child[0]);
    critbit_free_node(node->child[1]);
    free(node);
  } else {
    free(ptr);
  }
}

void
critbit_clear(critbit_tree * cb) {
  if(cb->root) {
    critbit_free_node(cb->root);
    cb->root = 0;
  }
}
