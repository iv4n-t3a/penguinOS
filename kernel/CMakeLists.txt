set(HEADERS
    compiler.h
    config.h
    drivers/init.h
    drivers/keyboard.h
    drivers/keyboard_hook.h
    drivers/ps2_keyboard.h
    libs/kernel_log.h
    libs/kernel_panic.h
    libs/stdio.h
    mem/buddy.h
    mem/buddy_util.h
    mem/freelist.h
    mem/kmalloc.h
    mem/slab.h
    mem/util.h
    log_boot_params.h
    shell/shell.h)

set(HEADERS_X86
    arch/x86/bochs.h
    arch/x86/config.h
    arch/x86/gdt.h
    arch/x86/idt.h
    arch/x86/io.h
    arch/x86/isr.h
    arch/x86/kmalloc_init.h
    arch/x86/pic.h)

set(HEADERS_DEBUG arch/debug/keyboard.h)

set(SOURCES
    drivers/init.c
    drivers/keyboard_hook.c
    drivers/ps2_keyboard.c
    libs/kernel_log.c
    libs/stdio.c
    log_boot_params.c
    mem/buddy.c
    mem/buddy_util.c
    mem/freelist.c
    mem/kmalloc.c
    mem/slab.c
    mem/util.c
    main.c
    shell/shell.c)

set(SOURCES_X86
    arch/x86/gdt.c
    arch/x86/idt.c
    arch/x86/init.c
    arch/x86/isr.c
    arch/x86/kmalloc_init.c
    arch/x86/pic.c
    arch/x86/ps2_keyboard.c
    arch/x86/vga.c)

set(SOURCES_DEBUG arch/debug/init.c arch/debug/keyboard.c arch/debug/main.c
                  arch/debug/ps2_keyboard.c arch/debug/vga.c)

if("${ARCH}" STREQUAL "x86")
  list(APPEND SOURCES ${SOURCES_X86})
  list(APPEND HEADERS ${HEADERS_X86})

  set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/kernel/linker_x86.ld")
  set(USE_LINKER_SCRIPT TRUE)
elseif("${ARCH}" STREQUAL "debug")
  list(APPEND SOURCES ${SOURCES_DEBUG})
  list(APPEND HEADERS ${HEADERS_DEBUG})
else()
  message(FATAL_ERROR "Unsupported architecture ${ARCH}")
endif()

add_executable(kernel.elf ${SOURCES} ${HEADERS})

target_include_directories(kernel.elf PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(kernel.elf core_lib)

if(${USE_LINKER_SCRIPT})
  target_link_options(kernel.elf PRIVATE "LINKER:-T${LINKER_SCRIPT}")
  target_link_options(kernel.elf PRIVATE
                      "LINKER:-Map=${PROJECT_SOURCE_DIR}/kernel.map")
  set_target_properties(kernel.elf PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT}")
endif()
