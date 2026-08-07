#ifndef KERNEL_DRIVERS_PCI_INIT_H
#define KERNEL_DRIVERS_PCI_INIT_H

#include "kernel/drivers/pci.h"

void init_pci_device_driver(int bus, int device, int function, pci_device_base_header_t);

#endif // #ifndef KERNEL_DRIVERS_PCI_INIT_H
