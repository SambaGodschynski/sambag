cmake_minimum_required(VERSION 3.15)
# CMake 4.x removed the legacy FindBoost module; use Boost's own CMake config.
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(sambag VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ── Options ───────────────────────────────────────────────────────────────────
option(SAMBAG_USE_LOG        "Enable boost::log support"              ON)
option(SAMBAG_VST_SUPPORT    "Build with VST2/VST3 SDK support"       ON)
option(SAMBAG_BUILD_TESTS    "Build CppUnit unit tests"               ON)
option(SAMBAG_BUILD_TESTAPP  "Build UI test applications (X11)"       ON)
# For VST plugins all dependencies should be linked statically so the .so
# needs no external DLLs / .so files in the plugin folder.
option(SAMBAG_STATIC_DEPS    "Prefer static linking of dependencies"  ON)

# ── Version from git ─────────────────────────────────────────────────────────
execute_process(
    COMMAND git --work-tree=${PROJECT_SOURCE_DIR} log --pretty=format:%h -n 1
    OUTPUT_VARIABLE SAMBAG_VERSION_BUILD
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(NOT SAMBAG_VERSION_BUILD)
    set(SAMBAG_VERSION_BUILD "unknown")
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/src/sambag/com/Config.h.in"
    "${PROJECT_BINARY_DIR}/src/sambag/com/Config.h"
)

# ── Platform detection ────────────────────────────────────────────────────────
if(APPLE)
    add_compile_definitions(DISCO_USE_COCOA RELEASE)
    find_library(COCOA_LIBRARY  Cocoa  REQUIRED)
    find_library(CARBON_LIBRARY Carbon REQUIRED)
elseif(UNIX)
    find_package(X11 REQUIRED)
    add_compile_definitions(DISCO_USE_X11)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed")
elseif(WIN32)
    add_compile_definitions(
        DISCO_USE_WIN32
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        SAMBAG_USE_WINDOWS_SHARED_MEMORY
        _WIN32_WINNT=0x0A00
    )
    if(MSVC)
        string(APPEND CMAKE_CXX_FLAGS_DEBUG   " /MTd /ZI")
        string(APPEND CMAKE_CXX_FLAGS_RELEASE " /MT")
    endif()
endif()

# Global compile definitions (silences MSVC/Boost warnings, harmless on GCC)
add_compile_definitions(
    BOOST_UBLAS_TYPE_CHECK=0
    BOOST_BIND_GLOBAL_PLACEHOLDERS
    BOOST_FILESYSTEM_VERSION=3
    _SCL_SECURE_NO_WARNINGS
    _CRT_SECURE_NO_WARNINGS
)
if(SAMBAG_USE_LOG)
    add_compile_definitions(SAMBAG_USE_LOG)
endif()

# ── Dependencies ──────────────────────────────────────────────────────────────
# On Linux/macOS we use pkg-config; on Windows vcpkg provides CMake config packages
if(NOT WIN32)
    find_package(PkgConfig REQUIRED)
endif()

# --- Boost ---
set(_BOOST_COMPONENTS
    filesystem system thread regex serialization
    date_time timer chrono program_options)
if(SAMBAG_USE_LOG)
    list(APPEND _BOOST_COMPONENTS log log_setup)
endif()
if(SAMBAG_STATIC_DEPS)
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost 1.55.0 REQUIRED COMPONENTS ${_BOOST_COMPONENTS})

# --- Cairo ---
if(WIN32)
    # The vcpkg cairo port uses Meson internally and does not always generate
    # cmake config files. Use find_library/find_path; vcpkg toolchain adds the
    # installed triplet's include/ and lib/ to the search paths automatically.
    find_path(CAIRO_INCLUDE_DIR NAMES cairo/cairo.h REQUIRED)
    find_library(CAIRO_LIBRARY  NAMES cairo        REQUIRED)
    find_library(PIXMAN_LIBRARY NAMES pixman-1     REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(ZLIB     REQUIRED)
    find_package(PNG      REQUIRED)
    find_library(BZ2_LIBRARY NAMES bz2 bzip2)

    add_library(_sambag_cairo INTERFACE)
    # cairo.h lives in include/cairo/cairo.h; add the cairo/ subdir so that
    # #include <cairo.h> (used throughout sambag) resolves correctly.
    target_include_directories(_sambag_cairo INTERFACE
        "${CAIRO_INCLUDE_DIR}"
        "${CAIRO_INCLUDE_DIR}/cairo"
    )
    target_link_libraries(_sambag_cairo INTERFACE
        "${CAIRO_LIBRARY}"
        "${PIXMAN_LIBRARY}"
        Freetype::Freetype
        ZLIB::ZLIB
        PNG::PNG
    )
    if(BZ2_LIBRARY)
        target_link_libraries(_sambag_cairo INTERFACE "${BZ2_LIBRARY}")
    endif()
    set(SAMBAG_CAIRO_TARGET _sambag_cairo)
else()
    pkg_check_modules(Cairo REQUIRED IMPORTED_TARGET cairo)
    set(SAMBAG_CAIRO_TARGET PkgConfig::Cairo)
endif()

# --- Lua ---
if(WIN32)
    # vcpkg port: lua  →  standard CMake FindLua variables
    find_package(Lua REQUIRED)
    add_library(_sambag_lua INTERFACE)
    target_include_directories(_sambag_lua INTERFACE ${LUA_INCLUDE_DIR})
    target_link_libraries(_sambag_lua INTERFACE ${LUA_LIBRARIES})
    set(SAMBAG_LUA_TARGET _sambag_lua)
else()
    pkg_check_modules(Lua lua5.4 IMPORTED_TARGET)
    if(NOT Lua_FOUND)
        pkg_check_modules(Lua lua5.2 IMPORTED_TARGET)
    endif()
    if(NOT Lua_FOUND)
        pkg_check_modules(Lua REQUIRED lua IMPORTED_TARGET)
    endif()
    set(SAMBAG_LUA_TARGET PkgConfig::Lua)
endif()

# SQLite3 is bundled as the amalgamation in src/sambag/cpsqlite/sqlite3.c
# (included by the custom sambag_sqlite.c). No system package needed.

# --- Loki (header-only) ---
# ext/loki is bundled for Windows; system package (libloki-dev) used on Linux
find_path(LOKI_INCLUDE_DIR loki/Typelist.h
    PATHS
        /usr/include /usr/local/include
        "${CMAKE_CURRENT_SOURCE_DIR}/ext/loki/include"
    DOC "Loki library headers")
if(NOT LOKI_INCLUDE_DIR)
    message(FATAL_ERROR
        "Loki headers not found.\n"
        "On Linux: sudo apt install libloki-dev\n"
        "On Windows: bundled ext/loki/include should be found automatically.")
endif()

# --- CppUnit (only needed for tests) ---
if(SAMBAG_BUILD_TESTS)
    if(WIN32)
        # vcpkg cppunit port may export a different target name; use find_library
        # to avoid cmake config / target-name ambiguity.
        find_path(CPPUNIT_INCLUDE_DIR NAMES cppunit/TestCase.h REQUIRED)
        find_library(CPPUNIT_LIBRARY   NAMES cppunit           REQUIRED)
        add_library(_sambag_cppunit INTERFACE)
        target_include_directories(_sambag_cppunit INTERFACE "${CPPUNIT_INCLUDE_DIR}")
        target_link_libraries(_sambag_cppunit INTERFACE "${CPPUNIT_LIBRARY}")
        set(SAMBAG_CPPUNIT_TARGET _sambag_cppunit)
    else()
        pkg_check_modules(CppUnit REQUIRED cppunit IMPORTED_TARGET)
        set(SAMBAG_CPPUNIT_TARGET PkgConfig::CppUnit)
    endif()
endif()

# --- VST2 / VST3 SDK (optional, user must set CLIBS env var) ---
if(SAMBAG_VST_SUPPORT)
    if(NOT DEFINED ENV{CLIBS})
        message(FATAL_ERROR
            "SAMBAG_VST_SUPPORT=ON requires the CLIBS environment variable "
            "pointing to the directory that contains vstsdk/VST3 subdirs.")
    endif()

    find_path(VSTSDK_INCLUDE_DIR public.sdk/source/vst2.x/audioeffect.h
        PATHS $ENV{CLIBS}/vstsdk $ENV{CLIBS}/vstsdk2.4
        DOC "VST2 SDK root")

    find_path(VSTSDK3_INCLUDE_DIR public.sdk/source/vst/vstaudioeffect.h
        PATHS $ENV{CLIBS}/VST3
        DOC "VST3 SDK root")

    if(NOT VSTSDK_INCLUDE_DIR OR NOT VSTSDK3_INCLUDE_DIR)
        message(FATAL_ERROR
            "VST2 or VST3 SDK not found under CLIBS=$ENV{CLIBS}\n"
            "Expected: $ENV{CLIBS}/vstsdk and $ENV{CLIBS}/VST3")
    endif()

    # __cdecl appears in VST2 public headers; neutralise it on non-MSVC
    if(NOT MSVC)
        add_compile_definitions("__cdecl=__attribute__((__cdecl__))")
    endif()
endif()

# ── Sub-directories ───────────────────────────────────────────────────────────
add_subdirectory(ext/tinyxml)
add_subdirectory(src)
