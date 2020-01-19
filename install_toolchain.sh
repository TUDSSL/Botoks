#!/bin/bash

MSP430_GCC_VERSION=${MSP430_GCC_VERSION:-8.3.0.16}
MSP430_GCC_OS=${MSP430_GCC_OS:-linux64}  # can be 'linux32' or 'macos' instead
INSTALL_PREFIX=${INSTALL_PREFIX:-~/ti}

cd ${INSTALL_PREFIX}

# Download toolchain and support files
echo "Downloading toolchain and support files..."
wget -q --show-progress \
    "http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/exports/msp430-gcc-${MSP430_GCC_VERSION}_${MSP430_GCC_OS}.tar.bz2"
wget -q --show-progress \
    "http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/exports/msp430-gcc-support-files-1.208.zip"

# Extract toolchain and support files
echo "Extracting toolchain and support files..."
tar xf msp430-gcc-${MSP430_GCC_VERSION}_${MSP430_GCC_OS}.tar.bz2
unzip -qq msp430-gcc-support-files-1.208.zip

# Rename toolchain to ${INSTALL_PREFIX}/msp430-gcc
mv msp430-gcc-${MSP430_GCC_VERSION}_${MSP430_GCC_OS} msp430-gcc

# Move support files into toolchain directory
mv msp430-gcc-support-files/include/ msp430-gcc/include/

# Remove compressed archives and unneeded support files
echo "Cleaning up..."
rm msp430-gcc-${MSP430_GCC_VERSION}_${MSP430_GCC_OS}.tar.bz2
rm msp430-gcc-support-files-1.208.zip
rm -rf msp430-gcc-support-files

cd -

echo "MSP430 toolchain installed!"
echo ""
echo "Now you can use '-DMSP430_TOOLCHAIN_DIR=${INSTALL_PREFIX}' when calling cmake"
