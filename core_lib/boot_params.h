#ifndef CORE_LIB_BOOT_PARAMS_H
#define CORE_LIB_BOOT_PARAMS_H

#include <stddef.h>
#include <stdint.h>

#define MEMORY_REGIONS_BUFFER_SIZE 16

typedef enum {
  RAM_TYPE_USABLE = 1,
  RAM_TYPE_RESERVED = 2,
  RAM_TYPE_ACPI_RECLAIMABLE = 3,
  RAM_TYPE_ACPI_NVS = 4,
  RAM_TYPE_BAD_MEMORY = 5
} x86_ram_type_t;

typedef struct {
  uint64_t base;
  uint64_t length;
  uint32_t type;
  uint32_t ACPI;
} x86_mementry_t;

typedef struct {
  x86_mementry_t memory_regions[MEMORY_REGIONS_BUFFER_SIZE];
  size_t memory_regions_count;
} x86_boot_params_t;

typedef struct {
  int boot_drive;
  void *stack_begin;

  void *kernel_begin;
  size_t kernel_size;

  x86_boot_params_t x86_boot_params;
} boot_params_t;

#endif // #ifndef CORE_LIB_BOOT_PARAMS_H
