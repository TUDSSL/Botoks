project(calib-accuracy VERSION 1.0.0 LANGUAGES C ASM)

set(DEPENDENCIES
    external/mspbase
    external/mspprintf
    external/mspspi
    libs/chrt
    )

# Source files of libraries and externals
foreach(dep ${DEPENDENCIES})
    list(APPEND DEP_SOURCES "${CMAKE_SOURCE_DIR}/${dep}/src/*.[cs]")
endforeach()

# List source files to be compiled
file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/*.[cs]"
    ${DEP_SOURCES}
    )

# Add executable target
foreach(i RANGE 3) 

set(exName "${PROJECT_NAME}-t${i}")
add_executable(${exName} ${SOURCES})

# Change target suffix to have <name>.out
set_target_properties(${exName} PROPERTIES SUFFIX ${OUTPUT_SUFFIX})

# Defines (-D)
target_compile_definitions(${exName}
    PUBLIC HWREV=4
    PUBLIC TIER=${i}
    )

# HWREV
# 4 = 2.0
# 3 = 1.2
# 2 = 1.0
# 1 = Dev board

# Compiler options for this project
target_compile_options(${exName}
    PUBLIC -mlarge -mdata-region=either -mcode-region=either
    PRIVATE -O0
    PRIVATE -g -gdwarf-3
    PRIVATE -Wall
    PRIVATE -std=c99
    PRIVATE -ffunction-sections -fdata-sections
    )

# Include directories of libraries and externals
foreach(dep ${DEPENDENCIES})
    target_include_directories(${exName}
        PRIVATE ${CMAKE_SOURCE_DIR}/${dep}/include)
endforeach()

# Linker options for this project
target_link_options(${exName}
    PUBLIC -mlarge -mdata-region=either -mcode-region=either
    PRIVATE -Wl,--gc-sections
    PRIVATE -Wl,-Map=${PROJECT_NAME}.map
    )

target_link_libraries(${exName}
    gcc
    c
    m
    )
    
# Print size of binary after linking
add_custom_command(TARGET ${exName} POST_BUILD
    COMMAND ${CMAKE_SIZE} ${exName}${OUTPUT_SUFFIX}
    )

# Install executable in Botoks/bin
install(TARGETS ${exName} DESTINATION bin)
    
endforeach(i RANGE 3)




