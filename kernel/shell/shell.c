#include "kernel/shell/shell.h"

#include <stddef.h>

#include "core_lib/string.h"
#include "kernel/config.h"
#include "kernel/libs/stdio.h"

size_t last_enterred_char;
char command_buffer[KERNEL_SHELL_BUFFER_SIZE];

void kernel_shell_init() {
  kprintf("%s", kernel_shell_prompt);

  last_enterred_char = 0;

  for (size_t i = 0; i < KERNEL_SHELL_BUFFER_SIZE; ++i) {
    command_buffer[i] = '\0';
  }
}

void strip_command_buffer() {
  size_t offset = 0;

  for (; offset < last_enterred_char; ++offset) {
    char c = command_buffer[offset];
    if (c != ' ' && c != '\0' && c != '\n') {
      break;
    }
  }

  for (size_t i = 0; i < KERNEL_SHELL_BUFFER_SIZE - offset; ++i) {
    command_buffer[i] = command_buffer[i + offset];
  }

  for (int i = KERNEL_SHELL_BUFFER_SIZE - 1; i > 0; --i) {
    char c = command_buffer[i];
    if (c != ' ' && c != '\0' && c != '\n') {
      break;
    }
    command_buffer[i] = '\0';
  }
}

void kernel_shell_run_command() {
  strip_command_buffer();

  if (pos_strcmp(command_buffer, "help", KERNEL_SHELL_BUFFER_SIZE)) {
    kprintf("Nothing can help you\n");
  } else if (pos_strcmp(command_buffer, "neofetch", KERNEL_SHELL_BUFFER_SIZE)) {
    kprintf("                  ==++==+\n"
            "               =+**%%@%%+*=+@#+\n"
            "               ***@-%%.:@@-@=\n"
            "               @@@@ *@@+@#+\n"
            "                @@@%%+=+*+=-#\n"
            "                :--::-:::=:#..\n"
            "                %%--+*::+=::%%+#.. %%%%@+=\n"
            "      ##%%    +=+*%%:--==-::#.*+%%@.  ##++\n"
            "     %%###=+@@:%%:+=:::::..*-:.-.%%%%    ##+.\n"
            "     %%%###@@:%%#%%##.....+:::...:@@     %%#*\n"
            "     .%%%%%%%%#%%*=@-==++-**#::--....:%%        .\n"
            "         @%%%%* =-----.*=.*.:.....::%%\n"
            "         =-   ---:::::--::......::%%\n"
            "         +    :--::::::::....:::::%%\n"
            "              @--:::::::::::::::::%%\n"
            "               --::::::::::::::::::\n"
            "               *-----------------=\n"
            "                --=============--\n"
            "                .+*************+.\n"
            "                  #%%%%%%    ..%%%%%%-\n"
            "                 .**#.     .-**.\n"
            "                  .. .     .. .\n"
            "You're using pinguing OS!?\n");
  } else if (command_buffer[0] != '\0') {
    kprintf("Unknown command\n");
  }

  kernel_shell_init();
}

void kernel_shell_process_char(keycode_t key) {
  switch (key.pressed_char) {
  case '\n':
    kprintf("%c", key.pressed_char);
    kernel_shell_run_command();
    break;
  case '\b':
    if (last_enterred_char != 0) {
      kprintf("%c", key.pressed_char);
      last_enterred_char -= 1;
      command_buffer[last_enterred_char] = '\0';
    }
    break;
  default:
    if (last_enterred_char < KERNEL_SHELL_BUFFER_SIZE - 1) {
      command_buffer[last_enterred_char] = key.pressed_char;
      last_enterred_char += 1;
      kprintf("%c", key.pressed_char);
    }
    break;
  }
}
