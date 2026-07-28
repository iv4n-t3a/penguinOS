#include "kernel/drivers/pci.h"

#include <stddef.h>
#include <stdint.h>

#include "core_lib/compiler.h"
#include "kernel/libs/kernel_log.h"

static const uint16_t k_invalid_vendor_id = 0xFFFF;
static const uint16_t k_pci_functions_per_device = 8;
static const uint16_t k_pci_devices_per_bus = 8;

typedef enum {
  PCI_HEADER_TYPE_STANDART = 0x0,
  PCI_HEADER_TYPE_PCI_TO_PCI_BRIDGE = 0x1,
  PCI_HEADER_TYPE_CARD_BUS_BRIDGE = 0x2,
} pci_header_type_t;

typedef enum {
  PCI_CLASS_UNCLASSIFIED = 0x00,
  PCI_CLASS_BRIDGE = 0x06,
} pci_device_class_t;

typedef enum {
  PCI_BRIDGE_SUBCLASS_PCI_TO_PCI_BRIDGE = 0x04,
} pci_bridge_subclass_t;

typedef struct {
  int register_offset : 8;
  int function_number : 3;
  int device_number : 5;
  int bus_number : 8;
  int _reseved : 7;
  bool enable : 1;
} PACKED pci_address_t;

typedef struct {
  uint16_t vendor_id;
  uint16_t device_id;
  uint16_t command;
  uint16_t status;
  uint8_t revision_id;
  uint8_t prog_if;
  uint8_t subclass;
  uint8_t class;
  uint8_t cache_line_size;
  uint8_t latency_timer;
  pci_header_type_t header_type : 7;
  bool multi_function : 1;
  uint8_t BIST;
} PACKED pci_device_base_header_t;

typedef struct {
  pci_device_base_header_t base_header;
  uint32_t base_address_0;
  uint32_t base_address_1;
  uint8_t primary_bus_number;
  uint8_t secondary_bus_number;
  uint8_t subordinate_bus_number;
  uint8_t secondary_latency_timer;
  // TODO
} PACKED pci_to_pci_bridge_header_t;

static void read_pci_header(int bus, int device, int function, uint32_t* out,
                            size_t read_bytes) {
  for (size_t i = 0; i * sizeof(uint32_t) < sizeof(pci_device_base_header_t);
       ++i) {
    pci_address_t address;

    address.bus_number = bus;
    address.device_number = device;
    address.function_number = function;
    address.register_offset = i * 4;
    address.enable = true;

    out[i] = pci_config_read(*(uint32_t*)&address);
  }
}

static pci_device_base_header_t get_base_pci_header(int bus, int device,
                                                    int function) {
  pci_device_base_header_t res;
  read_pci_header(bus, device, function, (uint32_t*)&res,
                  sizeof(pci_device_base_header_t));
  return res;
}

static pci_to_pci_bridge_header_t get_pci_to_pci_header(int bus, int device,
                                                        int function) {
  pci_to_pci_bridge_header_t res;
  read_pci_header(bus, device, function, (uint32_t*)&res,
                  sizeof(pci_to_pci_bridge_header_t));
  return res;
}

static void check_bus(int bus);

static void check_function(int bus, int device, int function) {
  pci_device_base_header_t header = get_base_pci_header(bus, device, function);

  if (header.vendor_id == k_invalid_vendor_id) {
    return;
  }

  LOG_INFO(
      "Found pci device bus=%d, device=%d, function=%d, "
      "class=%x, subclass=%x, vendor_id=%x\n",
      bus, device, function, header.class, header.subclass, header.vendor_id);

  if (header.class == PCI_CLASS_BRIDGE &&
      header.subclass == PCI_BRIDGE_SUBCLASS_PCI_TO_PCI_BRIDGE) {
    pci_to_pci_bridge_header_t header =
        get_pci_to_pci_header(bus, device, function);
    LOG_INFO("Found pci-to-pci bridge. From bus %d to %d\n", bus,
             header.secondary_bus_number);
    check_bus(header.secondary_bus_number);
  }
}

static void check_device(int bus, int device) {
  int function = 0;

  pci_device_base_header_t header = get_base_pci_header(bus, device, function);

  if (header.vendor_id == k_invalid_vendor_id) {
    return;
  }

  if (header.multi_function) {
    for (int function = 0; function < k_pci_functions_per_device; ++function) {
      check_function(bus, device, function);
    }
  } else {
    check_function(bus, device, 0);
  }
}

static void check_bus(int bus) {
  for (int device = 0; device < k_pci_devices_per_bus; ++device) {
    check_device(bus, device);
  }
}

void pci_scan_devices() {
  pci_device_base_header_t header = get_base_pci_header(0, 0, 0);

  if (header.multi_function) {
    LOG_INFO("Found multiple PCI host controllers\n");
    for (int function = 0; function < k_pci_functions_per_device; ++function) {
      pci_device_base_header_t header = get_base_pci_header(0, 0, function);

      if (header.vendor_id != k_invalid_vendor_id) {
        break;
      }

      check_bus(function);
    }
  } else {
    LOG_INFO("Found single PCI host controller\n");
    check_bus(0);
  }
}
