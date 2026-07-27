#include <stdint.h>

#include "core_lib/boot_params.h"
#include "core_lib/compiler.h"
#include "core_lib/string.h"
#include "kernel/config.h"
#include "kernel/libs/kernel_log.h"
#include "kernel/mem/pfalloc.h"

typedef struct {
  bool present : 1;
  bool write_enabled : 1;
  bool supervisor : 1;
  bool page_write_through : 1;
  bool page_cache_disabled : 1;
  bool accessed : 1;
  int _unused1 : 1;
  bool large_page_size_enabled : 1;
  int _unused2 : 4;
  int page_frame_number : 20;
} PACKED page_directory_entry_t;

typedef struct {
  bool present : 1;
  bool write_enabled : 1;
  bool supervisor : 1;
  bool page_write_through : 1;
  bool page_cache_disabled : 1;
  bool accessed : 1;
  bool dirty : 1;
  bool page_attribute_table : 1;
  bool global : 1;
  int _unused : 3;
  int page_frame_number : 20;
} PACKED page_table_entry_t;

static const size_t k_page_directory_entries =
    PAGE_SIZE / sizeof(page_directory_entry_t);
static const size_t k_page_frame_number_shift = 12;

static page_directory_entry_t* init_identity_map(size_t pages_count) {
  size_t used_pde_count =
      (pages_count + k_page_directory_entries - 1) / k_page_directory_entries;

  LOG_DEBUG("Identity map params: pages_count=%x, used_pde_count=%x\n", pages_count, used_pde_count);

  page_directory_entry_t* page_directory = pfalloc(1);
  page_table_entry_t* pte_arr = pfalloc(used_pde_count);

  pos_memset(page_directory, 0, PAGE_SIZE);
  pos_memset(pte_arr, 0, used_pde_count * PAGE_SIZE);

  for (size_t i = 0; i < pages_count; ++i) {
    pte_arr[i].page_frame_number = (i * PAGE_SIZE) >> k_page_frame_number_shift;
    pte_arr[i].present = true;
  }

  for (size_t i = 0; i < used_pde_count; ++i) {
    page_directory[i].page_frame_number =
        ((int)pte_arr + i * PAGE_SIZE) >> k_page_frame_number_shift;
    page_directory[i].present = true;
  }

  return page_directory;
}

static inline void load_page_directory(page_directory_entry_t* page_directory) {
  asm volatile("mov %%eax, %%cr3" : : "a"(page_directory));
}

static inline void enable_paging() {
  asm volatile(
      "movl %%cr0, %%eax \n\t"
      "orl $0x80000000, %%eax \n\t"
      "movl %%eax, %%cr0\n\t"
      :
      :
      : "eax");
}

void init_identity_paging(boot_params_t params) {
  LOG_DEBUG("Initializing identity paging map for first %x bytes of memory...\n", params.memory_total);
  page_directory_entry_t* page_directory = init_identity_map((params.memory_total + PAGE_SIZE - 1) / PAGE_SIZE);
  LOG_DEBUG("Identity paging map initialized.\n");

  LOG_DEBUG("Loading page directory...\n");
  load_page_directory(page_directory);
  LOG_DEBUG("Page directory loaded.\n");

  LOG_DEBUG("Enabling paging...\n");
  enable_paging();
  LOG_DEBUG("Paging enabled.\n");
}
