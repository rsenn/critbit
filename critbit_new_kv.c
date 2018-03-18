#include "critbit_internal.h"
#include <string.h>

size_t
critbit_new_kv(const char *key, size_t keylen, void * value, size_t len, void * out) {
  char * dst = (char*)out;
  if(dst != key) {
    memcpy(dst, key, keylen);
  }
  dst[keylen] = 0;
  memcpy(dst + keylen + 1, value, len);
  return len + keylen + 1;
}
