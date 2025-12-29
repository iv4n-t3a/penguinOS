#include "kernel/arch/x86/kmalloc_init.h"
#include "core_lib/boot_params.h"
#include "kernel/mem/kmalloc.h"
#include "kernel/mem/pfalloc.h"

// This functions assumes that large usable block exists and
// kernel placed at the beggining of it. Stack must not overlap it
// TODO: Propper allocator initialization
void kmalloc_init_x86(boot_params_t params) {
  x86_mementry_t *entries = params.x86_boot_params.memory_regions;
  size_t entries_cnt = params.x86_boot_params.memory_regions_count;

  void *base = NULL;
  size_t length = 0;

  for (size_t i = 0; i < entries_cnt; ++i) {
    if (entries[i].length > length) {
      base = (void *)entries[i].base;
      length = entries[i].length;
    }
  }

  void *kernel_end = params.kernel_begin + params.kernel_size;

  if (kernel_end > base) {
    base = kernel_end;
  }

  pfalloc_init(base, length);
  kmalloc_init(get_pfalloc_buddy());
}
