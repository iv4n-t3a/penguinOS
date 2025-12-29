#include <stdbool.h>
#include <stdint.h>

#include "boot/stage2/compiler.h"
#include "boot/stage2/detect_memory.h"
#include "boot/stage2/error.h"
#include "boot/stage2/fat.h"
#include "boot/stage2/find_partition.h"
#include "boot/stage2/read_disk.h"
#include "boot/stage2/read_elf.h"
#include "core_lib/boot_params.h"
#include "core_lib/elf.h"
#include "core_lib/mbr.h"
#include "core_lib/string.h"
#include "core_lib/vga.h"

void CDECL bootmain(int boot_drive) {
  boot_params_t params = {0};

  params.boot_drive = boot_drive;
  params.stack_begin = (void *)0x8000;

  terminal_initialize(80, 25);

  ERR_HANDLE_MAIN(detect_memory(&params));

  MBR_partition_table_t *pt;
  int partition;

  ERR_HANDLE_MAIN(find_partition(boot_drive, &pt, &partition));
  ERR_HANDLE_MAIN(FAT_init(boot_drive, pt->entries + partition));

  void (*main)(boot_params_t);
  size_t ksize = 0;

  ERR_HANDLE_MAIN(read_elf("KERNEL  ELF", (void *)&main, &ksize));

  params.kernel_begin = main;
  params.kernel_size = ksize;

  main(params);

  ERR_HANDLE_MAIN(ERR_KERNEL_EXITED);
}
