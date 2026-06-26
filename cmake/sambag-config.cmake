cmake_minimum_required(VERSION 3.15)
include(CMakeFindDependencyMacro)

get_filename_component(_SAMBAG_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)

# ── Transitive dependencies ───────────────────────────────────────────────────
if(WIN32)
    # Boost (vcpkg static)
    set(Boost_USE_STATIC_LIBS ON)
    find_dependency(Boost 1.55.0 COMPONENTS
        filesystem system thread regex serialization
        date_time timer chrono program_options log log_setup)

    # Cairo (vcpkg, no cmake config — found via toolchain paths)
    find_library(SAMBAG_CAIRO_LIB   NAMES cairo    REQUIRED)
    find_library(SAMBAG_PIXMAN_LIB  NAMES pixman-1 REQUIRED)
    find_dependency(Freetype REQUIRED)
    find_dependency(ZLIB REQUIRED)
    find_dependency(PNG REQUIRED)
    find_library(SAMBAG_BZ2_LIB        NAMES bz2 bzip2)
    find_library(SAMBAG_FONTCONFIG_LIB NAMES fontconfig)
    find_dependency(EXPAT)

    # Lua (vcpkg, standard FindLua variables)
    find_dependency(Lua REQUIRED)
else()
    find_dependency(PkgConfig REQUIRED)
    pkg_check_modules(SAMBAG_CAIRO REQUIRED IMPORTED_TARGET cairo)
    find_dependency(Boost 1.55.0 COMPONENTS
        filesystem system thread regex serialization
        date_time timer chrono program_options log log_setup)
    pkg_check_modules(SAMBAG_LUA REQUIRED IMPORTED_TARGET lua)
endif()

# ── ticpp (bundled tinyxml wrapper) ──────────────────────────────────────────
if(NOT TARGET sambag::ticpp)
    add_library(sambag::ticpp STATIC IMPORTED)
    find_library(_SAMBAG_TICPP_LIB ticpp_ticpp
        PATHS "${_SAMBAG_PREFIX}/lib" NO_DEFAULT_PATH REQUIRED)
    set_target_properties(sambag::ticpp PROPERTIES
        IMPORTED_LOCATION "${_SAMBAG_TICPP_LIB}")
endif()

# ── sambag::sambag ────────────────────────────────────────────────────────────
if(NOT TARGET sambag::sambag)
    find_library(_SAMBAG_LIB sambag
        PATHS "${_SAMBAG_PREFIX}/lib" NO_DEFAULT_PATH REQUIRED)

    add_library(sambag::sambag STATIC IMPORTED)
    set_target_properties(sambag::sambag PROPERTIES
        IMPORTED_LOCATION "${_SAMBAG_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${_SAMBAG_PREFIX}/include"
        INTERFACE_COMPILE_DEFINITIONS
            "BOOST_UBLAS_TYPE_CHECK=0;BOOST_BIND_GLOBAL_PLACEHOLDERS;BOOST_FILESYSTEM_VERSION=3"
    )

    if(WIN32)
        target_link_libraries(sambag::sambag INTERFACE
            sambag::ticpp
            Boost::filesystem Boost::system Boost::thread Boost::regex
            Boost::serialization Boost::date_time Boost::timer
            Boost::chrono Boost::program_options Boost::log Boost::log_setup
            "${SAMBAG_CAIRO_LIB}" "${SAMBAG_PIXMAN_LIB}"
            Freetype::Freetype ZLIB::ZLIB PNG::PNG
        )
        if(SAMBAG_BZ2_LIB)
            target_link_libraries(sambag::sambag INTERFACE "${SAMBAG_BZ2_LIB}")
        endif()
        if(SAMBAG_FONTCONFIG_LIB)
            target_link_libraries(sambag::sambag INTERFACE "${SAMBAG_FONTCONFIG_LIB}")
        endif()
        if(TARGET EXPAT::EXPAT)
            target_link_libraries(sambag::sambag INTERFACE EXPAT::EXPAT)
        endif()
        # Lua: FindLua doesn't produce imported targets, wrap it
        add_library(sambag::lua INTERFACE IMPORTED)
        set_property(TARGET sambag::lua PROPERTY
            INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")
        set_property(TARGET sambag::lua PROPERTY
            INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}")
        target_link_libraries(sambag::sambag INTERFACE sambag::lua)
        # Windows system libs needed at link time
        target_link_libraries(sambag::sambag INTERFACE ws2_32 mswsock)
    else()
        target_link_libraries(sambag::sambag INTERFACE
            sambag::ticpp
            ${Boost_LIBRARIES}
            PkgConfig::SAMBAG_CAIRO
            PkgConfig::SAMBAG_LUA
            rt pthread dl
        )
    endif()
endif()

set(sambag_FOUND TRUE)
