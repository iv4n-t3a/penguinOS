#include "kernel/drivers/init.h"
#include "core_lib/boot_params.h"
#include "core_lib/vga.h"
#include "kernel/arch/x86/config.h"
#include "kernel/arch/x86/gdt.h"
#include "kernel/arch/x86/idt.h"
#include "kernel/arch/x86/isr.h"
#include "kernel/arch/x86/kmalloc_init.h"
#include "kernel/arch/x86/pic.h"
#include "kernel/libs/kernel_log.h"

void arch_init(boot_params_t params) {
  terminal_initialize(k_terminal_width, k_terminal_height);
  LOG_INFO("Terminal initialized.\n");

  LOG_INFO("Loaded GDT...\n");
  GDT_load();
  LOG_INFO("GDT loaded.\n");

  LOG_INFO("Initializing PIC...\n");
  PIC_init(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
  PIC_mask_irq(k_pic_mask);
  LOG_INFO("PIC initialized.\n");

  LOG_INFO("Loading IDT...\n");
  IDT_load(idt_handlers, sizeof(idt_handlers) / sizeof(idtentry_t));
  LOG_INFO("IDT loaded.\n");

  LOG_INFO("Initializing drivers...\n");
  init_drivers(params);
  LOG_INFO("Drivers initialized.\n");

  LOG_INFO("Initializing allocator...\n");
  kmalloc_init_x86(params);
  LOG_INFO("Allocator initialized.\n");
}
