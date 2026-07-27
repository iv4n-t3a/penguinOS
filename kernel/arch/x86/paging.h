#ifndef KERNEL_ARCH_X86_PAGING_H
#define KERNEL_ARCH_X86_PAGING_H

#include "core_lib/boot_params.h"

void init_identity_paging(boot_params_t params);

#endif // #ifndef KERNEL_ARCH_X86_PAGING_H
