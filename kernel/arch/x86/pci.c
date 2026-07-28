#include "kernel/drivers/pci.h"

#include "kernel/arch/x86/config.h"
#include "kernel/arch/x86/io.h"

uint32_t pci_config_read(uint32_t address) {
  port_out_32(k_pci_config_address_port, address);
  io_wait();
  return port_in_32(k_pci_config_data_port);
}

void pci_config_write(uint32_t address, uint32_t value) {
  port_out_32(k_pci_config_address_port, address);
  io_wait();
  port_out_32(k_pci_config_data_port, value);
}
