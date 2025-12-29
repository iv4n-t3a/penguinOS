#ifndef KERNEL_MEM_KMALLOC_H
#define KERNEL_MEM_KMALLOC_H

#include "kernel/mem/buddy.h"
#include <stddef.h>

void kmalloc_init(buddy_alloc_t *alloc);

void *kmalloc(size_t size);

void kfree(void *ptr);

#endif // #ifndef KERNEL_MEM_KMALLOC_H
