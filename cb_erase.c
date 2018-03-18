#include "critbit_internal.h"
#include <stdlib.h>
#include <string.h>

int cb_erase(critbit_tree * cb, const void * key, size_t keylen)
{
  void **iter = &cb->root;
  void *ptr = *iter;
  unsigned char * bytes = (unsigned char *)key;

  if (!cb->root) return 0;

  if (decode_pointer(&ptr)==EXTERNAL_NODE) {
    free(ptr);
    cb->root = 0;
    return CB_SUCCESS;
  }

  for (;;) {
    struct critbit_node *parent = (struct critbit_node *)ptr;
    int branch, type;

    branch = (keylen<=parent->byte) ? 0 : ((1+((bytes[parent->byte]|parent->mask)&0xFF))>>8);

    ptr = parent->child[branch];
    type = decode_pointer(&ptr);
    if (type==INTERNAL_NODE) {
      iter = &parent->child[branch];
    } else {
      void * str;
      size_t len;
      from_external_node(ptr, &str, &len);
      if (len==keylen && memcmp(key, str, len)==0) {
        free(ptr);
        *iter = parent->child[1-branch];
        return CB_SUCCESS;
      }
      return 0;
    }
  }
}
