#!/bin/bash

mkimage() {
  cmake -S . -B $BUILDDIR -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN -DARCH:STRING=$ARCH -DTARGET:STRING=BOOTABLE
  make -C $BUILDDIR
  BUILDDIR=$BUILDDIR ARCH=$ARCH ./mkimage/mkimage_${ARCH}.sh
}

run_tests() {
    cmake -S . -B $BUILDDIR -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN -DTARGET:STRING=TESTS -DARCH:STRING=$ARCH
    make -C $BUILDDIR
    ./$BUILDDIR/tests/unit_tests --gtest_color=yes
}

run_qemu() {
    qemu-system-i386 -debugcon stdio -m 4G -drive file=$BOOTABLE,format=raw
}

run_bochs_debugger() {
    bochs-debugger -debugger
}

build_tools() {
    make -C third_party
}

show_help() {
    echo "Usage: $0 [COMMAND] [OPTION]..."
    echo ""
    echo "Commands:"
    echo "  mkimage         Creates bootable CD image"
    echo "  tests           Run tests"
    echo "  qemu            Run in QEMU"
    echo "  bochs-debugger  Run in Bochs"
    echo "  tools           Build tools (cross-compiler, etc)"
    echo ""
    echo "Options:"
    echo "  --arch ARCH           Target architecture (One of the following: x86)"
    echo "  --toolchain PATH      Path to CMake toolchain file"
    echo "  --builddir PATH       Path to CMake build dir"
    echo "  -h, --help            Show this help message"
}

ARCH="x86"

# Parse arguments
POSITIONAL=()
while [[ $# -gt 0 ]]; do
    case "$1" in
        mkimage|tests|qemu|bochs-debugger|tools)
            COMMAND="$1"
            shift
            ;;
        --arch)
            ARCH="$2"
            shift 2
            ;;
        --toolchain)
            TOOLCHAIN="$2"
            shift 2
            ;;
        --builddir)
            BUILDDIR="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            POSITIONAL+=("$1")
            shift
            ;;
    esac
done

if [[ -z "$COMMAND" ]]; then
    show_help
    exit 1
fi

if [[ "$COMMAND" == "tests" ]]; then
    ARCH="debug"
    if [[ -z "$BUILDDIR" ]]; then
        BUILDDIR="build_tests"
    fi
fi

if [[ -z "$TOOLCHAIN" ]]; then
    TOOLCHAIN="toolchain_$ARCH.cmake"
fi

if [[ -z "$BUILDDIR" ]]; then
    BUILDDIR="build_$ARCH"
fi

if [[ "$ARCH" != "x86" && "$ARCH" != "debug" ]]; then
    echo "Error: unsupported architecture $ARCH."
    exit 1
fi

if [[ ! -f "$TOOLCHAIN" ]]; then
    echo "Error: Toolchain file '$TOOLCHAIN' does not exist."
    exit 1
fi

if [[ -z "$BOOTABLE" ]]; then
    BOOTABLE=$BUILDDIR/bootable.dd
fi

set -e
mkdir -p $BUILDDIR

case "$COMMAND" in
    mkimage)
        echo "Creating bootable CD for $ARCH"
        mkimage
        ;;
    tests)
        echo "Running tests"
        run_tests
        ;;
    qemu)
        echo "Running in QEMU for $ARCH"
        run_qemu
        ;;
    bochs-debugger)
        echo "Running in Bochs debugger for $ARCH"
        run_bochs_debugger
        ;;
    tools)
        echo "Building tools for $ARCH"
        build_tools
        ;;
    *)
        show_help
        exit 1
        ;;
esac
