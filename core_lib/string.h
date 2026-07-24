#ifndef CORE_LIB_STRING_H
#define CORE_LIB_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t pos_strlen(const char *str);
void pos_itos(uint32_t num, char *str);

bool pos_strcmp(const char *str1, const char *str2, size_t size);
void *pos_memcpy(char *dst, const char *src, size_t count);
void *pos_memset(void *mem, uint8_t val, size_t count);
void *pos_memmove(void *dst, const void *src, size_t count);
int pos_memcmp(const void *lhs, const void *rhs, size_t count);

#endif // #ifndef CORE_LIB_STRING_H
