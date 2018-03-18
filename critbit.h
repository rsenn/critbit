/*
Copyright (c) 2012, Enno Rehling <enno@eressea.de>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**/

#ifndef _CRITBITT_H
#define _CRITBITT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct critbit_tree {
  void * root;
} critbit_tree;

#define CB_SUCCESS 1

#define CRITBIT_TREE() { 0 }

int critbit_insert(critbit_tree * cb, const void * key, size_t keylen);
const void * critbit_find(critbit_tree * cb, const void * key, size_t keylen);
int critbit_erase(critbit_tree * cb, const void * key, size_t keylen);
int critbit_find_prefix(critbit_tree * cb, const void * key, size_t keylen, const void ** results, int numresults, int offset);
int critbit_foreach(critbit_tree * cb, const void * key, size_t keylen, int (*match_cb)(const void * match, const void * key, size_t keylen, void *), void *data);
void critbit_clear(critbit_tree * cb);

#define cb_insert_str(cb, key) \
  critbit_insert(cb, (void *)key, strlen(key)+1)
#define cb_find_str(cb, key) \
  (const char *)critbit_find(cb, (void *)key, strlen(key)+1)
#define cb_erase_str(cb, key) \
  critbit_erase(cb, (void *)key, strlen(key)+1)
#define cb_find_prefix_str(cb, key, results, numresults, offset) \
  critbit_find_prefix(cb, (void *)key, strlen(key), results, numresults, offset)

#define CB_KV_SIZE(keylen, datalen) (keylen+datalen+1)
size_t critbit_new_kv(const char *key, size_t keylen, void * value, size_t len, void * out);
void critbit_get_kv(const void *kv, void * value, size_t len);

#ifdef __cplusplus
}
#endif
#endif
