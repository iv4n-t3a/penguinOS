# Penguin OS

**This project isn't complete OS at the moment, but it can boot and `neofetch`**

## Features

- two-stage legacy BIOS boot
- fat16 filesystem support
- buddy, slab memory allocator
- ps/2 keyboard driver
- vga display driver

![demo](assets/demo.jpg)

## Download release and run

Go to releases page and download `bootable.dd` for the latest release. Run with QEMU:

    qemu-system-i386 -debugcon stdio -m 256M -drive file=bootable.dd,format=raw

Or even write to real usb-drive for running on real hardware

**WARNING**: PenguinOS is a hobby project, the author has no responsibility for what happens to your device after running PenguinOS.

    sudo dd if=bootable.dd of=/dev/sdX # Put your drive device path

Then stick the drive and find the boot option in boot-menu. This probably won't work on modern devices, unless your turn bios-compatibility options. But the proper way is to find old 32-bit laptop.

## Build

### Install deps

#### deps

- gcc
- mtools
- cmake
- m4 (shipped with gcc on most distros, but might not)
- libasan (for tests only)

#### install on fedora

    sudo dnf install -y gcc mtools cmake m4 libasan
    sudo dnf install -y qemu qemu-kvm qemu-system-i386 # qemu for running on vm

#### install on ubuntu

    sudo apt install -y gcc mtools cmake m4 libasan8
    sudo apt install -y qemu-kvm qemu-system-i386 # qemu for running on vm

### Build cross-compiler (this may take a time)

    ./build.sh tools

### Build bootable image

    ./build.sh mkimage

bootable will lie in build_x86/bootable.dd

### Run PenguinOS on QEMU

    ./build.sh qemu

### Run tests

    ./build.sh tests
