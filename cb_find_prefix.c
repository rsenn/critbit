#include "critbit_internal.h"
#include <string.h>
#include <assert.h>

static int cb_find_prefix_i(void * ptr, const void * key, size_t keylen, const void ** results, int numresults, int * offset, int next)
{
  assert(next<=numresults);
  if (next==numresults) {
    return next;
  } else if (decode_pointer(&ptr)==INTERNAL_NODE) {
    struct critbit_node * node = (struct critbit_node *)ptr;
    next = cb_find_prefix_i(node->child[0], key, keylen, results, numresults, offset, next);
    if (next<numresults) {
      next = cb_find_prefix_i(node->child[1], key, keylen, results, numresults, offset, next);
    }
  } else {
    /* reached an external node */
    const char * str;
    void * vptr;
    size_t len;

    from_external_node(ptr, &vptr, &len);
    str = vptr;
    if (len>=keylen && memcmp(key, str, keylen)==0) {
      if (*offset>0) {
        --*offset;
      } else {
        results[next++] = str;
      }
    }
  }
  return next;
}

int cb_find_prefix(critbit_tree * cb, const void * key, size_t keylen, const void ** results, int numresults, int offset)
{
  if (numresults>0) {
    void *top = cb_find_top_i(cb, key, keylen);
    if (top) {
      /* recursively add all children except the ones from [0-offset) of top to the results */
      return cb_find_prefix_i(top, key, keylen, results, numresults, &offset, 0);
    }
  }
  return 0;
}
