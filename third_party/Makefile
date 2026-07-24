include ../config.mk

TOOLCHAIN_PREFIX=$(shell pwd)
TARGET=i686-elf


all: \
	.gcc-${GCC_VERSION}-build-succeed \
	.binutils-${BINUTILS_VERSION}-build-succeed
.PHONY: all


gcc: .gcc-${GCC_VERSION}-build-succeed
.PHONY: gcc

binutils: .binutils-${BINUTILS_VERSION}-build-succeed
.PHONY: binutils

mpfr: .mpfr-${MPFR_VERSION}-build-succeed
.PHONY: mpfr

gmp: .gmp-${GMP_VERSION}-build-succeed
.PHONY: gmp

mpc: .mpc-${MPC_VERSION}-build-succeed
.PHONY: mpc


clean:
	find . -mindepth 1 -maxdepth 1 ! -name Makefile -exec rm -rf {} +
.PHONY: clean

clean-all: clean-archieves clean-sources clean-binaries
.PHONY: clean-all

clean-archieves:
	rm *.tar *.gz *.xz
.PHONY: clean-archieves

clean-sources:
	rm -rf gcc-* binutils-* gmp-* mpfr-* mpc-*
.PHONY: clean-sources

clean-binaries:
	rm -rf bin *-elf include lib libexec share *-build-succeed
.PHONY: clean-binaries


.gcc-${GCC_VERSION}-build-succeed: \
		.gmp-${GMP_VERSION}-build-succeed           \
		.mpc-${MPC_VERSION}-build-succeed           \
		.mpfr-${MPFR_VERSION}-build-succeed         \
		.binutils-${BINUTILS_VERSION}-build-succeed \
		| gcc-${GCC_VERSION}
	echo ${TARGET}
	mkdir -p gcc-${GCC_VERSION}-build
	cd gcc-${GCC_VERSION}-build; \
	../gcc-${GCC_VERSION}/configure \
		--prefix="${TOOLCHAIN_PREFIX}"         \
		--with-gmp="${TOOLCHAIN_PREFIX}"       \
		--with-mpfr="${TOOLCHAIN_PREFIX}"      \
		--with-mpc="${TOOLCHAIN_PREFIX}"      \
		--target=${TARGET}                     \
		--disable-nls                          \
		--enable-languages=c,c++               \
		--without-headers                      \
		--disable-hosted-libstdcxx
	make -C gcc-${GCC_VERSION}-build -j${BUILD_TOOLS_JOBS} all-gcc
	make -C gcc-${GCC_VERSION}-build -j${BUILD_TOOLS_JOBS} all-target-libgcc
	make -C gcc-${GCC_VERSION}-build -j${BUILD_TOOLS_LINK_JOBS} all-target-libstdc++-v3
	make -C gcc-${GCC_VERSION}-build -j${BUILD_TOOLS_JOBS} install-gcc
	make -C gcc-${GCC_VERSION}-build -j${BUILD_TOOLS_JOBS} install-target-libgcc
	make -C gcc-${GCC_VERSION}-build -j${BUILD_TOOLS_LINK_JOBS} install-target-libstdc++-v3
	touch .gcc-${GCC_VERSION}-build-succeed

gcc-${GCC_VERSION}: gcc-${GCC_VERSION}.tar.xz
	tar xvf $<

gcc-${GCC_VERSION}.tar.xz:
	wget https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz


.binutils-${BINUTILS_VERSION}-build-succeed: | binutils-${BINUTILS_VERSION}
	mkdir -p binutils-${BINUTILS_VERSION}-build
	cd binutils-${BINUTILS_VERSION}-build; \
	../binutils-${BINUTILS_VERSION}/configure \
	    --prefix="${TOOLCHAIN_PREFIX}"        \
			--target=${TARGET}        \
			--with-sysroot            \
			--disable-nls             \
			--disable-werror
	make -C binutils-${BINUTILS_VERSION}-build -j ${BUILD_TOOLS_JOBS}
	make -C binutils-${BINUTILS_VERSION}-build -j ${BUILD_TOOLS_JOBS} install
	touch .binutils-${BINUTILS_VERSION}-build-succeed

binutils-${BINUTILS_VERSION}: binutils-${BINUTILS_VERSION}.tar.xz
	tar xvf $<

binutils-${BINUTILS_VERSION}.tar.xz:
	wget https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz


.mpc-${MPC_VERSION}-build-succeed: \
		.mpfr-${MPFR_VERSION}-build-succeed \
		| mpc-${MPC_VERSION}
	mkdir -p mpc-${MPC_VERSION}-build
	cd mpc-${MPC_VERSION}-build; \
	../mpc-${MPC_VERSION}/configure \
		--prefix="${TOOLCHAIN_PREFIX}" \
		--with-gmp="${TOOLCHAIN_PREFIX}"  \
		--with-mpfr="${TOOLCHAIN_PREFIX}" \
		--disable-shared
	make -C mpc-${MPC_VERSION}-build
	make -C mpc-${MPC_VERSION}-build install
	touch .mpc-${MPC_VERSION}-build-succeed

mpc-${MPC_VERSION}: mpc-${MPC_VERSION}.tar.xz
	tar -xvf $<

mpc-${MPC_VERSION}.tar.xz:
	wget https://ftp.gnu.org/gnu/mpc/mpc-${MPC_VERSION}.tar.xz


.gmp-${GMP_VERSION}-build-succeed: | gmp-${GMP_VERSION}
	mkdir -p gmp-${GMP_VERSION}-build
	cd gmp-${GMP_VERSION}-build; \
	CFLAGS="-O2 -m64 -std=gnu17" ../gmp-${GMP_VERSION}/configure \
		--prefix="${TOOLCHAIN_PREFIX}" \
		--disable-assembly \
		--disable-shared
	make -C gmp-${GMP_VERSION}-build -j ${BUILD_TOOLS_JOBS}
	make -C gmp-${GMP_VERSION}-build -j ${BUILD_TOOLS_JOBS} install
	touch .gmp-${GMP_VERSION}-build-succeed

gmp-${GMP_VERSION}: gmp-${GMP_VERSION}.tar.xz
	tar xvf $<

gmp-${GMP_VERSION}.tar.xz:
	wget https://ftp.gnu.org/gnu/gmp/gmp-${GMP_VERSION}.tar.xz


.mpfr-${MPFR_VERSION}-build-succeed: | mpfr-${MPFR_VERSION}
	mkdir -p mpfr-${MPFR_VERSION}-build
	cd mpfr-${MPFR_VERSION}-build; \
	../mpfr-${MPFR_VERSION}/configure      \
		--prefix="${TOOLCHAIN_PREFIX}" \
		--with-gmp="${TOOLCHAIN_PREFIX}" \
		--disable-shared
	make -C mpfr-${MPFR_VERSION}-build -j ${BUILD_TOOLS_JOBS}
	make -C mpfr-${MPFR_VERSION}-build -j ${BUILD_TOOLS_JOBS} install
	touch .mpfr-${MPFR_VERSION}-build-succeed

mpfr-${MPFR_VERSION}: mpfr-${MPFR_VERSION}.tar.xz
	tar xvf $<

mpfr-${MPFR_VERSION}.tar.xz:
	wget https://ftp.gnu.org/gnu/mpfr/mpfr-${MPFR_VERSION}.tar.xz
