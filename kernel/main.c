#include "core_lib/boot_params.h"
#include "kernel/libs/kernel_log.h"
#include "kernel/shell/shell.h"
#include "kernel/drivers/init.h"

// kernel/arch/.../init.c
void arch_init(boot_params_t);

void kmain(boot_params_t params) {
  arch_init(params);

  LOG_INFO("Pinguin OS kernel initialized\n");

  LOG_INFO("Initializing drivers...\n");
  init_drivers(params);
  LOG_INFO("Drivers initialized.\n");

  LOG_INFO("Initializing shell...\n");
  kernel_shell_init();
  LOG_INFO("Shell initialized\n");

  for (;;) {
  }
}
