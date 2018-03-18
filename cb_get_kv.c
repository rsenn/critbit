#include "critbit_internal.h"
#include <string.h>

void cb_get_kv(const void *kv, void * value, size_t len)
{
  const char * key = (const char *)kv;
  size_t keylen = strlen(key)+1;
  memcpy(value, key+keylen, len);
}
