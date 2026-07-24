#include "boot/stage2/read_disk.h"

#include <stddef.h>
#include <stdint.h>

#include "boot/stage2/bios_calls.h"
#include "boot/stage2/build_config.h"
#include "core_lib/string.h"

// Use mediator buffer since bios can't work with address greater than 0xF'FFFF
uint8_t buffer[READ_DISK_MEDIATOR_BUFFER_SIZE];

error_t load_sector(int drive, uint64_t lba) {
  DAPack dap;

  if (init_DAPack(&dap, buffer, lba, 1)) {
    return NEW_ERR(ERR_DAPACK_INIT_ERROR);
  }

  if (BIOS_read_disk(&dap, drive)) {
    return NEW_ERR(ERR_BIOS_CALL_ERROR);
  }

  return NEW_ERR(ERR_SUCCESS);
}

error_t read_disk(int drive, void *dst, uint64_t lba, uint16_t sectors) {
  for (size_t i = 0; i < sectors; ++i) {
    ERR_HANDLE_SUBROUTINE(load_sector(drive, lba + i));
    pos_memcpy(dst, buffer, READ_DISK_MEDIATOR_BUFFER_SIZE);
    dst += READ_DISK_MEDIATOR_BUFFER_SIZE;
  }

  return NEW_ERR(ERR_SUCCESS);
}
