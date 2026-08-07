#ifndef KERNEL_DRIVERS_PCI_H
#define KERNEL_DRIVERS_PCI_H

#include <stdbool.h>
#include <stdint.h>

#include "core_lib/compiler.h"

typedef enum {
  PCI_HEADER_TYPE_STANDART = 0x0,
  PCI_HEADER_TYPE_PCI_TO_PCI_BRIDGE = 0x1,
  PCI_HEADER_TYPE_CARD_BUS_BRIDGE = 0x2,
} pci_header_type_t;

typedef enum {
  PCI_CLASS_UNCLASSIFIED = 0x00,
  PCI_CLASS_MASS_STORAGE_CONTROLLER = 0x01,
  PCI_CLASS_BRIDGE = 0x06,
} pci_device_class_t;

typedef enum {
  PCI_MASS_STORAGE_CONTROLLER_IDE_CONTROLLER = 0x01,
} pci_mass_storage_controller_subclass_t;

typedef enum {
  PCI_BRIDGE_SUBCLASS_PCI_TO_PCI_BRIDGE = 0x04,
} pci_bridge_subclass_t;

typedef struct {
  uint16_t vendor_id;
  uint16_t device_id;
  uint16_t command;
  uint16_t status;
  uint8_t revision_id;
  uint8_t prog_if;
  uint8_t dev_subclass;
  pci_device_class_t dev_class : 8;
  uint8_t cache_line_size;
  uint8_t latency_timer;
  pci_header_type_t header_type : 7;
  bool multi_function : 1;
  uint8_t BIST;
} PACKED pci_device_base_header_t;

void pci_scan_devices();

// Arch-specific symbols, defined in kernel/arch/.../pci.c
uint32_t pci_config_read(uint32_t address);

void pci_config_write(uint32_t address, uint32_t value);

#endif // #ifndef KERNEL_DRIVERS_PCI_H
