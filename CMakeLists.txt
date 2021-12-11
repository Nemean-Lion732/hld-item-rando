cmake_minimum_required(VERSION 3.12)

# Define project.
project(         "itemrando" # Mod name (required)
    VERSION      "0.1.0" # Mod version (required)
    DESCRIPTION  "" # Mod description (optional)
    HOMEPAGE_URL "" # Mod homepage (optional)
    LANGUAGES C
)
set(PROJECT_MINIMUM_MRE_VERSION # Mod minimum MRE version (optional)
    ""
)
set(PROJECT_AUTHORS # List of mod authors (optional)
    "Nemean_Lion732"
)

# Update this as you add new source files.
set(SRC
    src/moddef.c
    src/crate.c
    src/logic.c
    src/vanilla_handler.c
)

# Prepare mod information for inclusion in ModInfo.json.
list(JOIN PROJECT_AUTHORS "\",\"" PROJECT_AUTHORS_JSON)

# Set cmake defaults.
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "lib${PROJECT_NAME}-${PROJECT_VERSION}"
        CACHE PATH "..." FORCE
    )
endif()

# Include CPack.
set(CPACK_GENERATOR "TXZ")
set(CPACK_PACKAGE_FILE_NAME "lib${PROJECT_NAME}-${PROJECT_VERSION}")
include(CPack)

# Include GenerateExportHeader.
include(GenerateExportHeader)

# Include AERMRE.
set(AERMRE_SCRIPT_REL "aer/mre/lib/cmake/AERMRETargets.cmake")
include("$ENV{AER_GAMEDIR}/${AERMRE_SCRIPT_REL}")

# Instantiate ModInfo template.
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/ModInfo.json.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ModInfo.json"
    @ONLY
)

# Add mod shared library target.
add_library(${PROJECT_NAME} SHARED ${SRC})
generate_export_header(${PROJECT_NAME}
    BASE_NAME MOD
    EXPORT_FILE_NAME include/export.h
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    C_VISIBILITY_PRESET hidden
    CMAKE_VISIBILITY_INLINES_HIDDEN TRUE
)
target_include_directories(${PROJECT_NAME}
    PRIVATE include "${CMAKE_CURRENT_BINARY_DIR}/include"
)
target_compile_options(${PROJECT_NAME}
    PRIVATE -m32 -Wall -Wextra -Werror=implicit-function-declaration -Wfatal-errors
)
target_link_options(${PROJECT_NAME}
    PRIVATE -m32 -rdynamic
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE AERMRE::aermre
)

# Add installation target.
install(TARGETS
    ${PROJECT_NAME}
    LIBRARY DESTINATION "lib"
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ModInfo.json"
    DESTINATION "."
)
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt"
        DESTINATION "."
    )
endif()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    install(DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        DESTINATION "."
    )
endif()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ACKNOWLEDGMENTS")
    install(DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/ACKNOWLEDGMENTS"
        DESTINATION "."
    )
endif()