#include "boot/stage2/read_elf.h"

#include "boot/stage2/fat.h"
#include "core_lib/elf.h"

void sort_elf_segments(ELF_program_header32_t *segments, int count) {
  // TODO
}

error_t read_elf(char *name, void **main, size_t *size) {
  FAT_file_t elf_file;
  ELF_header32_t header;
  *size = 0;

  ERR_HANDLE_SUBROUTINE(FAT_open(&elf_file, name));
  ERR_HANDLE_SUBROUTINE(
      FAT_read(&elf_file, (uint8_t *)&header, sizeof(ELF_header32_t)));

  // TODO: Verify other elf parameters;
  if (header.magic[0] != 0x7F) {
    return NEW_ERR(ERR_SIGNATURE_VERIFICATION_FAILED);
  }

  size_t header_table_size = header.program_header_table_entry_size *
                             header.program_header_table_entries_count;

  ELF_program_header32_t
      header_table[header_table_size / sizeof(ELF_program_header32_t)];

  ERR_HANDLE_SUBROUTINE(
      FAT_seek(&elf_file, header.program_header_table_offset - elf_file.pos));

  ERR_HANDLE_SUBROUTINE(
      FAT_read(&elf_file, (uint8_t *)header_table, header_table_size));

  sort_elf_segments(header_table, header.program_header_table_entries_count);

  for (size_t i = 0; i < header.program_header_table_entries_count; ++i) {
    // Bootloader requires kernel sections to be placed sequently in ram
    // otherwise gaps won't be unusable
    size_t end_addr = header_table[i].offset + header_table[i].memsz;
    if (*size < end_addr) {
      *size = end_addr;
    }

    if (!header_table[i].filesz) {
      // TODO: Propper setup for uninitialized data
      continue;
    }

    ERR_HANDLE_SUBROUTINE(
        FAT_seek(&elf_file, header_table[i].offset - elf_file.pos));
    ERR_HANDLE_SUBROUTINE(FAT_read(&elf_file, (uint8_t *)header_table[i].paddr,
                                   header_table[i].memsz));
  }

  *main = (void *)header.program_entry_offset;

  return NEW_ERR(ERR_SUCCESS);
}
