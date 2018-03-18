#include "critbit_internal.h"
#include <string.h>
#include <assert.h>

static int cb_less(const struct critbit_node * a, const struct critbit_node * b)
{
  return a->byte<b->byte || (a->byte==b->byte && a->mask < b->mask);
}

int decode_pointer(void ** ptr)
{
  ptrdiff_t numvalue = (char*)*ptr - (char*)0;
  if (numvalue&1) {
    *ptr = (void*)(numvalue-1);
    return EXTERNAL_NODE;
  }
  return INTERNAL_NODE;
}

void from_external_node(void * ptr, void **key, size_t *keylen)
{
  unsigned char * bytes = (unsigned char *)ptr;
#ifndef NDEBUG
  ptrdiff_t numvalue = bytes - (unsigned char*)0;
  assert(numvalue && (numvalue&1)==0);
  assert(key && keylen);
#endif
  memcpy(keylen, bytes, sizeof(size_t));
  *key = bytes+sizeof(size_t);
}

struct critbit_node * make_internal_node(void)
{
  struct critbit_node *node = (struct critbit_node *)malloc(sizeof(struct critbit_node));
  return node;
}
static void * make_external_node(const void * key, size_t keylen)
{
  char * data = (char *)malloc(sizeof(size_t) + keylen);
#ifndef NDEBUG
  ptrdiff_t numvalue = (char *)data - (char*)0;
  assert((numvalue&1)==0);
#endif
  assert(keylen);
  memcpy(data, &keylen, sizeof(size_t));
  memcpy(data+sizeof(size_t), key, keylen);
  return (void*)(data+1);
}

int cb_insert(critbit_tree * cb, const void * key, size_t keylen)
{
  assert(cb);
  assert(key);
  if (!cb->root) {
    cb->root = make_external_node(key, keylen);
    return 1;
  } else {
    void ** iter = &cb->root;
    struct critbit_node * prev = 0;
    for (;;) {
      void * ptr = *iter;
      if (decode_pointer(&ptr)==INTERNAL_NODE) {
        struct critbit_node * node = (struct critbit_node *)ptr;
        unsigned char * bytes = (unsigned char *)key;
        int branch = (keylen<=node->byte) ? 0 : ((1+((bytes[node->byte]|node->mask)&0xFF))>>8);
        iter = &node->child[branch];
        prev = node;
      } else {
        unsigned char *iptr, *bytes = (unsigned char *)key, *ikey = bytes;
        void * vptr;
        unsigned int mask, branch;
        unsigned int byte = 0;
        size_t len;
        struct critbit_node * node = make_internal_node();

        from_external_node(ptr, &vptr, &len);

        for (iptr=vptr;byte<keylen && byte<len && *ikey==*iptr;) {
          ++ikey;
          ++iptr;
          ++byte;
        }

        if (byte==keylen && byte==len) {
          return 0; /* duplicate entry */
        }
        node->byte = byte;
        mask = *ikey ^ *iptr; /* these are all the bits that differ */
        mask |= mask>>1;
        mask |= mask>>2;
        mask |= mask>>4; /* now, every bit up to the MSB is set to 1 */
        mask = (mask&~(mask>>1))^0xFF;
        node->mask = (unsigned char)mask;

        /* find the right place to insert, iff prev's crit-bit is later in the string than new crit-bit */
        if (prev && cb_less(node, prev)) {
          for (iter = &cb->root;;) {
            ptr = *iter;
            if (decode_pointer(&ptr)==INTERNAL_NODE) {
              struct critbit_node * next = (struct critbit_node *)ptr;
              if (cb_less(next, node)) {
                branch = ((1+((bytes[next->byte]|next->mask)&0xFF))>>8);
                iter = &next->child[branch];
              } else {
                break;
              }
            } else {
              assert(!"should never get here");
            }
          }
        }

        branch = ((1+((*ikey|node->mask)&0xFF))>>8);
        node->child[branch] = make_external_node(key, keylen);
        node->child[1-branch] = *iter;
        *iter = (void *)node;

        return CB_SUCCESS;
      }
    }
  }
}

