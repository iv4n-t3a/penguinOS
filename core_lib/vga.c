// This code is forked from https://wiki.osdev.org/Bare_Bones

#include "core_lib/vga.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "core_lib/string.h"

size_t terminal_width;
size_t terminal_height;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_initialize(size_t width, size_t height) {
  terminal_width = width;
  terminal_height = height;
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t *)0xB8000;
  terminal_clear();
}

int coordinates_to_index(int x, int y) { return y * terminal_width + x; }

void terminal_move_cursor(int x, int y) {
  terminal_row = x;
  terminal_column = y;
}

int terminal_get_row() { return terminal_row; }

int terminal_get_column() { return terminal_column; }

int terminal_get_width() { return terminal_width; }

int terminal_get_height() { return terminal_height; }

void terminal_clear() {
  terminal_row = 0;
  terminal_column = 0;

  for (size_t x = 0; x < terminal_width; ++x) {
    for (size_t y = 0; y < terminal_height; ++y) {
      terminal_putentryat(' ', terminal_color, x, y);
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  terminal_buffer[coordinates_to_index(x, y)] = vga_entry(c, color);
}

void terminal_putchar(char c) {
  switch (c) {
  case '\0':
    break;
  case '\b':
    if (terminal_column == 0) {
      terminal_column = terminal_width;
      if (terminal_row != 0) {
        terminal_row -= 1;
      }
    } else {
      terminal_column -= 1;
    }
    terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
    break;
  case '\r':
    terminal_column = 0;
    break;
  case '\n':
    ++terminal_row;
    terminal_column = 0;

    if (terminal_row == terminal_height) {
      terminal_scroll_down();
    }
    break;
  default:
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == terminal_width) {
      terminal_column = 0;
      if (++terminal_row == terminal_height) {
        terminal_scroll_down();
      }
    }
    break;
  }
}

void terminal_scroll_down() {
  for (size_t x = 0; x < terminal_width; ++x) {
    for (size_t y = 1; y < terminal_height; ++y) {
      terminal_buffer[coordinates_to_index(x, y - 1)] =
          terminal_buffer[coordinates_to_index(x, y)];
    }
  }

  for (size_t x = 0; x < terminal_width; ++x) {
    terminal_putentryat(' ', terminal_color, x, terminal_height - 1);
  }

  terminal_row -= 1;
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
  terminal_write(data, pos_strlen(data));
}

void terminal_writeint(uint32_t num) {
  char str[11];
  pos_itos(num, str);
  terminal_writestring(str);
}
