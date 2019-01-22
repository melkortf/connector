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

set (CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall")
set (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -march=pentium3 -mmmx -msse")
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

#add_subdirectory (dbus)
add_subdirectory (src)
