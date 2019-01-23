find_package (Qt5 5.5 REQUIRED COMPONENTS Core DBus)
set (CMAKE_AUTOMOC ON)

add_library (connector MODULE
    connector.h
    gameserver.h
    srcdswrapper.h

    connector.cpp
    gameserver.cpp
    srcdswrapper.cpp
)

configure_file (config.h.in config.h)
target_sources (connector PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/config.h)

qt5_add_dbus_adaptor (gameserver_DBUS_ADAPTOR org.morgoth.connector.gameserver.xml gameserver.h morgoth::GameServer)
target_sources (connector PRIVATE ${gameserver_DBUS_ADAPTOR})
set_property (SOURCE ${gameserver_DBUS_ADAPTOR} PROPERTY SKIP_AUTOMOC ON)

target_compile_options (connector PUBLIC -m32)
target_link_options (connector PUBLIC -m32 -Wl,--no-gnu-unique -fuse-ld=gold -static-libstdc++ -static-libgcc)

set_source_files_properties (connector.cpp srcdswrapper.cpp PROPERTIES
    COMPILE_FLAGS "\
        -fpermissive\
        -Wno-unknown-pragmas\
        -Wno-unused-parameter\
        -Wno-gnu-zero-variadic-macro-arguments\
        -Wno-expansion-to-defined\
        -Wno-unused-variable\
        -Wno-zero-length-array\
        -Wno-gnu-redeclared-enum\
        -Wno-extra-semi\
        -Wno-gnu-anonymous-struct\
        -Wno-nested-anon-types\
        -Wno-unused-local-typedef\
        -Wno-unused-private-field\
        -Wno-mismatched-tags\
        -Wno-ignored-qualifiers\
        -Wno-unused-local-typedefs\
        "
    COMPILE_DEFINITIONS "GNUC;POSIX;LINUX;_LINUX;_WCHAR_T_DEFINED;RAD_TELEMETRY_DISABLED;NO_MALLOC_OVERRIDE"
)

target_include_directories (connector PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SourceSdk2013}/mp/src/common
    ${SourceSdk2013}/mp/src/public
    ${SourceSdk2013}/mp/src/public/tier0
    ${SourceSdk2013}/mp/src/public/tier1
    ${SourceSdk2013}/mp/src/game/shared
    ${SourceSdk2013}/mp/src/game/server
)

set_target_properties (connector PROPERTIES
    PREFIX ""
    OUTPUT_NAME morgoth-connector
    POSITION_INDEPENDENT_CODE OFF
    CXX_VISIBILITY_PRESET hidden
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
        Qt5::DBus
)

