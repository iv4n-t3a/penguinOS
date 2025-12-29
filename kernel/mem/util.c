#include "kernel/mem/util.h"
#include "kernel/config.h"

void *align_addr_to_page_size_up(void *ptr) {
  return ptr + (PAGE_SIZE - (size_t)ptr % PAGE_SIZE);
}

void *align_addr_to_page_size_down(void *ptr) {
  return ptr - (size_t)ptr % PAGE_SIZE;
}

size_t align_size_to_page_size_up(size_t ptr) {
  return ptr + (PAGE_SIZE - (size_t)ptr % PAGE_SIZE);
}

size_t align_size_to_page_size_down(size_t ptr) {
  return ptr - (size_t)ptr % PAGE_SIZE;
}

size_t min_upper_power_of_2(size_t size) {
  size_t p = 0;

  while ((1ull << p) < size) {
    p += 1;
  }

  return 1ull << p;
}

size_t max_lower_power_of_2(size_t size) {
  size_t p = 0;

  while ((1ull << (p + 1)) <= size) {
    p += 1;
  }

  return 1ull << p;
}

size_t log2_upper(size_t size) {
  size_t p = 0;

  while ((1ull << p) < size) {
    p += 1;
  }

  return p;
}

size_t log2_lower(size_t size) {
  size_t p = 0;

  while ((1ull << (p + 1)) <= size) {
    p += 1;
  }

  return p;
}

size_t pages_count(void *begin, void *end) {
  begin = align_addr_to_page_size_up(begin);
  end = align_addr_to_page_size_down(end);
  return (end - begin) / PAGE_SIZE;
}

size_t dist(void *ptr1, void *ptr2) {
  if (ptr1 > ptr2) {
    return (uint8_t *)ptr1 - (uint8_t *)ptr2;
  } else {
    return (uint8_t *)ptr2 - (uint8_t *)ptr1;
  }
}
