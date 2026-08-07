#include "kernel/drivers/pci/init.h"
#include "kernel/drivers/pci.h"
#include "kernel/drivers/pci/ide.h"
#include "kernel/libs/kernel_log.h"

static void init_pci_mass_storage_controller_device_driver(int bus, int device, int function, pci_device_base_header_t header) {
  switch ((pci_mass_storage_controller_subclass_t)header.dev_subclass) {
    case PCI_MASS_STORAGE_CONTROLLER_IDE_CONTROLLER:
      LOG_INFO("Found IDE device at bus=%d, device=%d, function=%d. Initializing driver...\n", bus, device, function);
      init_ide_driver(bus, device, function, header);
      LOG_INFO("IDE driver for device at bus=%d, device=%d, function=%d initialized.\n", bus, device, function);
      break;
    default:
      LOG_INFO("No driver for mass storage device controller device subclass=%d.\n", header.dev_subclass);
      break;
  }
}

void init_pci_device_driver(int bus, int device, int function, pci_device_base_header_t header) {
  switch (header.dev_class) {
    case PCI_CLASS_MASS_STORAGE_CONTROLLER:
      init_pci_mass_storage_controller_device_driver(bus, device, function, header);
      break;
    default:
      LOG_INFO("No driver for PCI device class=%d found.\n", header.dev_class);
      break;
  }
}
