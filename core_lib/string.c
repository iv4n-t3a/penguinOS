#include "core_lib/string.h"

size_t pos_strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

void pos_itos(int32_t num, char *str) {
  if (num >= 0) {
    pos_utos(num, str);
  } else {
    str[0] = '-';
    pos_utos(-num, &str[1]);
  }
}

void pos_utos(uint32_t num, char *str) {
  if (num == 0) {
    str[0] = '0';
    str[1] = '\0';
    return;
  }

  uint32_t denum = 1;

  while (num / denum >= 10) {
    denum *= 10;
  }

  size_t i = 0;
  while (denum != 0) {
    str[i] = num / denum % 10 + '0';
    denum /= 10;
    i += 1;
  }
  str[i] = '\0';
}

bool pos_strcmp(const char *str1, const char *str2, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    if (str1[i] != str2[i]) {
      return false;
    }
    if (str1[i] == '\0') {
      return true;
    }
  }
  return true;
}

void *pos_memcpy(char *dst, const char *src, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    dst[i] = src[i];
  }
  return dst;
}

void *pos_memset(void *mem, uint8_t val, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    *(char *)(mem + i) = val;
  }
  return mem;
}

void *pos_memmove(void *dst, const void *src, size_t count) {
  char *d = (char *)dst;
  const char *s = (const char *)src;

  if (d == s || count == 0) {
    return dst;
  }

  if (d < s) {
    for (size_t i = 0; i < count; ++i) {
      d[i] = s[i];
    }
  } else {
    for (size_t i = count; i != 0; --i) {
      d[i - 1] = s[i - 1];
    }
  }

  return dst;
}

int pos_memcmp(const void *lhs, const void *rhs, size_t count) {
  const unsigned char *a = (const unsigned char *)lhs;
  const unsigned char *b = (const unsigned char *)rhs;

  for (size_t i = 0; i < count; ++i) {
    if (a[i] != b[i]) {
      return (int)a[i] - (int)b[i];
    }
  }

  return 0;
}

#if !__STDC_HOSTED__
// Freestanding builds still implicitly rely on these (e.g. struct
// zero-initialization lowers to memset, struct copies to memcpy), so the
// standard names must be provided even without a libc. Only declared when
// compiled freestanding (__STDC_HOSTED__ == 0) so hosted builds (e.g. tests)
// keep using the real libc's versions instead of clashing with these.

void *memcpy(void *dst, const void *src, size_t count) {
  return pos_memcpy((char *)dst, (const char *)src, count);
}

void *memset(void *mem, int val, size_t count) {
  return pos_memset(mem, (uint8_t)val, count);
}

void *memmove(void *dst, const void *src, size_t count) {
  return pos_memmove(dst, src, count);
}

int memcmp(const void *lhs, const void *rhs, size_t count) {
  return pos_memcmp(lhs, rhs, count);
}

#endif // #if !__STDC_HOSTED__
