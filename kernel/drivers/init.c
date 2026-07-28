#include "kernel/drivers/init.h"

#include "kernel/drivers/ps2_keyboard.h"
#include "kernel/drivers/pci.h"
#include "kernel/libs/kernel_log.h"

void init_drivers(boot_params_t params) {
  LOG_INFO("Initializing PS/2 keyboard driver...\n");
  ps2_keyboard_init();
  LOG_INFO("PS/2 keyboard driver initialized\n");

  LOG_INFO("Scaning for pci devices...\n");
  pci_scan_devices();
  LOG_INFO("Pci device scanned.\n");
}
