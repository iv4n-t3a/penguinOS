#ifndef KERNEL_DRIVERS_PCI_H
#define KERNEL_DRIVERS_PCI_H

#include <stdbool.h>
#include <stdint.h>

void pci_scan_devices();

// Arch-specific symbols, defined in kernel/arch/.../pci.c
uint32_t pci_config_read(uint32_t address);

void pci_config_write(uint32_t address, uint32_t value);

#endif // #ifndef KERNEL_DRIVERS_PCI_H
