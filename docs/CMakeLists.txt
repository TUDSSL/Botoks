find_package(Doxygen)

# Doxygen configuration
set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_XML YES)

set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
set(DOXYGEN_SHOW_INCLUDE_FILES NO)
set(DOXYGEN_SORT_MEMBER_DOCS NO)
set(DOXYGEN_REPEAT_BRIEF NO)

# Add Doxygen target and prepare Doxyfile
doxygen_add_docs(doxygen
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/apps
    )

# Sphinx configuration
set(SPHINX_BUILD_COMMAND sphinx-build)
set(SPHINX_BUILDER html)
set(SPHINX_OUTPUT_DIR ${SPHINX_BUILDER})

# Prepare Sphinx conf.py file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in
    ${CMAKE_CURRENT_BINARY_DIR}/conf.py
    @ONLY
    )

# Add Sphinx target
add_custom_target(sphinx
    COMMAND
        ${SPHINX_BUILD_COMMAND} -q
        -b ${SPHINX_BUILDER}
        -c ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/${SPHINX_OUTPUT_DIR}
    DEPENDS doxygen
    COMMENT "Build Sphinx HTML documentation"
    )

# Target to build docs and copy output here
add_custom_target(docs
    # COMMAND
        # ${CMAKE_COMMAND} -E copy_directory
        # ${CMAKE_CURRENT_BINARY_DIR}/${SPHINX_OUTPUT_DIR}
        # ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS sphinx
    )
