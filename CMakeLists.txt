cmake_minimum_required (VERSION 3.10 FATAL_ERROR)
set (CMAKE_SYSTEM_PROCESSOR "i686")
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
project (connector)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_BUILD_TYPE Debug)
else ()
    set(CMAKE_BUILD_TYPE Release)
endif ()

# find version
if (NOT connector_VERSION)
    file (READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION connector_VERSION)
    string (STRIP ${connector_VERSION} connector_VERSION)

    include (GetGitRevisionDescription)
    get_git_head_revision(GIT_REFSPEC GIT_SHA1)

    if (NOT "${GIT_SHA1}" STREQUAL "GITDIR-NOTFOUND")
        # shorten SHA
        string (SUBSTRING "${GIT_SHA1}" 0 10 GIT_HASH)
        set (connector_VERSION "${connector_VERSION}-${GIT_HASH}-git")
    endif ()
endif ()

set (CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall -Wextra -Wpedantic")
set (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set (CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined -Wl,--exclude-libs,ALL")
else ()
    set (CMAKE_SHARED_LINKER_FLAGS "-Wl,-undefined,error")
endif ()

set (CMAKE_CXX_STANDARD 14)

find_path (SourceSdk2013
    NAMES "engine/iserverplugin.h"
    PATHS "mp/src/public"
)

if (SourceSdk2013)
    message (STATUS "Source SDK located at ${SourceSdk2013}")
else ()
    message (FATAL_ERROR "Source SDK not found")
endif ()

add_library (connector MODULE
    connector.h
    connector.cpp
    eventstream.h
    eventstream.cpp
    gameevent.h
    gameevent.cpp
)

configure_file (config.h.in config.h)
target_sources (connector PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/config.h)

target_compile_options (connector
    PUBLIC
        -m32
        -march=pentium3
        -mmmx
        -msse
        -Wno-unknown-pragmas
        -Wno-unused-parameter
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-expansion-to-defined
        -Wno-unused-variable
        -Wno-zero-length-array
        -Wno-gnu-redeclared-enum
        -Wno-extra-semi
        -Wno-gnu-anonymous-struct
        -Wno-nested-anon-types
        -Wno-unused-local-typedef
        -Wno-unused-private-field
        -Wno-mismatched-tags
        -Wno-ignored-qualifiers
)

target_link_options (connector
    PUBLIC
        -m32
)

target_include_directories (connector PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SourceSdk2013}/mp/src/public
    ${SourceSdk2013}/mp/src/public/tier0
    ${SourceSdk2013}/mp/src/public/tier1
)

target_compile_definitions (connector PUBLIC
    GNUC
    POSIX
    LINUX
    _LINUX
    _WCHAR_T_DEFINED
)

set_target_properties (connector PROPERTIES
    PREFIX ""
    OUTPUT_NAME morgoth-connector
    POSITION_INDEPENDENT_CODE OFF
)

target_link_directories(connector
    PUBLIC
        ${SourceSdk2013}/mp/src/lib/public/linux32/
)

target_link_libraries (connector
    PUBLIC
        tier0
        vstdlib
        ${SourceSdk2013}/mp/src/lib/public/linux32/tier1.a
)
