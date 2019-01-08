cmake_minimum_required (VERSION 3.10 FATAL_ERROR)
project (connector)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_BUILD_TYPE Debug)
else ()
    set(CMAKE_BUILD_TYPE Release)
endif ()

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
    connector.cpp
)

target_include_directories (connector PUBLIC ${SourceSdk2013}/mp/src/public)
