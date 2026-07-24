#include "core_lib/stdio.h"

#include <stdarg.h>
#include <stddef.h>

#include "core_lib/string.h"

void generic_putstr(void (*pchar)(char), const char *str) {
  for (size_t i = 0; str[i] != '\0'; ++i) {
    pchar(str[i]);
  }
}

typedef enum {
  STATE_NORMAL,
  STATE_SPECIFIER,
} state_t;

static void print_int(void (*pchar)(char), int num) {
  char str[11];
  pos_itos(num, str);
  generic_putstr(pchar, str);
}

static void print_hex(void (*pchar)(char), int num) {
  pchar('0');
  pchar('x');

  if (num == 0) {
    pchar('0');
    return;
  }

  uint32_t denum = 1;

  while (num / denum >= 16) {
    denum *= 16;
  }

  size_t i = 0;
  while (denum != 0) {
    int digit = num / denum % 16;
    pchar(digit < 10 ? digit + '0' : digit + 'A' - 10);
    denum /= 16;
  }
}

static void process_char_normal(state_t *state, void (*pchar)(char), char c,
                         va_list *args) {
  switch (c) {
  case '%':
    *state = STATE_SPECIFIER;
    break;
  default:
    pchar(c);
    break;
  }
}

static void process_char_specifier(state_t *state, void (*pchar)(char), char c,
                            va_list *args) {
  // TODO: Segfault on arch debug
  switch (c) {
  case 'd':
  case 'i':
    int num = va_arg(*args, int);
    print_int(pchar, num);
    break;
  case 'x':
    int hex = va_arg(*args, int);
    print_hex(pchar, hex);
    break;
  case 'c':
    char c = va_arg(*args, int);
    pchar(c);
    break;
  case 's':
    const char *str = va_arg(*args, char *);
    generic_putstr(pchar, str);
    break;
  case '%':
    pchar('%');
    break;
  default:
    break;
  }
  *state = STATE_NORMAL;
}

static void process_char(state_t *state, void (*pchar)(char), char c, va_list *args) {
  switch (*state) {
  case STATE_NORMAL:
    process_char_normal(state, pchar, c, args);
    break;
  case STATE_SPECIFIER:
    process_char_specifier(state, pchar, c, args);
    break;
  }
}

void generic_printf(void (*pchar)(char), const char *fmt, va_list args) {
  state_t state = STATE_NORMAL;

  for (size_t i = 0; fmt[i] != '\0'; ++i) {
    process_char(&state, pchar, fmt[i], (va_list *)&args);
  }
}
