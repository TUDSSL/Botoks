# CMake toolchain for the MSP430FR microcontroller

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR msp430)

set(DEVICE "msp430fr5994" CACHE STRING "")
string(TOUPPER ${DEVICE} DEVICE_DEFINE)
set(DEVICE_DEFINE "__${DEVICE_DEFINE}__")

set(OUTPUT_SUFFIX ".out" CACHE STRING "")

# Linker script
set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/config/${DEVICE}.ld")

# Path to toolchain installation
set(TOOLCHAIN_PATH "$ENV{MSP430_TOOLCHAIN_PATH}")

# Compilers and tools
set(CMAKE_C_COMPILER    "${TOOLCHAIN_PATH}/bin/msp430-elf-gcc")
set(CMAKE_CXX_COMPILER  "${TOOLCHAIN_PATH}/bin/msp430-elf-g++")
set(CMAKE_AR            "${TOOLCHAIN_PATH}/bin/msp430-elf-ar")
set(CMAKE_LINKER        "${TOOLCHAIN_PATH}/bin/msp430-elf-ld")
set(CMAKE_NM            "${TOOLCHAIN_PATH}/bin/msp430-elf-nm")
set(CMAKE_OBJDUMP       "${TOOLCHAIN_PATH}/bin/msp430-elf-objdump")
set(CMAKE_STRIP         "${TOOLCHAIN_PATH}/bin/msp430-elf-strip")
set(CMAKE_RANLIB        "${TOOLCHAIN_PATH}/bin/msp430-elf-ranlib")
set(CMAKE_SIZE          "${TOOLCHAIN_PATH}/bin/msp430-elf-size")

# General compiler flags
add_compile_options(
    -MD
    -mmcu=${DEVICE}
    -D${DEVICE_DEFINE}
    )

# Toolchain include directories
include_directories(
    ${TOOLCHAIN_PATH}/include
    ${TOOLCHAIN_PATH}/msp430-elf/include
    ${PROJECT_SOURCE_DIR}/config
    ${PROJECT_SOURCE_DIR}/platforms
    )

# Device linker flags
add_link_options(
    -T${LINKER_SCRIPT}
    -L${TOOLCHAIN_PATH}/include
    -mmcu=${DEVICE}
    -D${DEVICE_DEFINE}
    #-Wl,-Map=${PROJECT_NAME}.map
    )
