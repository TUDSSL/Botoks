cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_TOOLCHAIN_FILE "config/msp430-toolchain.cmake")

project(botoks LANGUAGES C ASM)

# Check that toolchain environment variable exists
if(NOT DEFINED ENV{MSP430_TOOLCHAIN_PATH})
    message(FATAL_ERROR
        "Environment variable MSP430_TOOLCHAIN_PATH must be defined"
        )
endif()

# Check that toolchain path is correct
if(NOT EXISTS "$ENV{MSP430_TOOLCHAIN_PATH}/bin/msp430-elf-gcc")
    message(FATAL_ERROR
        "Environment variable MSP430_TOOLCHAIN_PATH must point to the root of "
        "the MSP430 toolchain"
        )
endif()

# Prefix path to install executables
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

add_subdirectory(apps)

# Only generate build files for docs if explicitely set BUILD_DOCS as a
# command-line argument, as building docs requires Doxygen, Sphinx and other
# Sphinx-related dependencies
if(BUILD_DOCS)
    add_subdirectory(docs EXCLUDE_FROM_ALL)
endif()
