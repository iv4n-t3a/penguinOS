#include "boot/stage2/detect_memory.h"

#include <stddef.h>

#include "boot/stage2/bios_calls.h"
#include "boot/stage2/error.h"

error_t detect_memory_E820(boot_params_t *params) {
  int call_id = 0;
  E820_mementry_t entry;

  for (size_t i = params->x86_boot_params.memory_regions_count;
       i < MEMORY_REGIONS_BUFFER_SIZE; ++i) {
    entry.ACPI = 1;

    if (BIOS_detect_memory_E820(&entry, &call_id)) {
      return NEW_ERR(ERR_BIOS_CALL_ERROR);
    }

    params->x86_boot_params.memory_regions[i].base = entry.base;
    params->x86_boot_params.memory_regions[i].length = entry.length;
    params->x86_boot_params.memory_regions[i].type = entry.type;
    params->x86_boot_params.memory_regions[i].ACPI = entry.ACPI;

    if (call_id == 0) {
      params->x86_boot_params.memory_regions_count = i;
      return NEW_ERR(ERR_SUCCESS);
    }
  }

  return NEW_ERR(ERR_BUFFER_OVERFLOW);
}

error_t detect_memory(boot_params_t *params) {
  ERR_HANDLE_SUBROUTINE(detect_memory_E820(params));
}
