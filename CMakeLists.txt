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
    )
    if(MSVC)
        string(APPEND CMAKE_CXX_FLAGS_DEBUG   " /MTd /ZI")
        string(APPEND CMAKE_CXX_FLAGS_RELEASE " /MT")
    endif()
endif()

# Global compile definitions (silences MSVC/Boost warnings, harmless on GCC)
add_compile_definitions(
    BOOST_UBLAS_TYPE_CHECK=0
    BOOST_FILESYSTEM_VERSION=3
    _SCL_SECURE_NO_WARNINGS
    _CRT_SECURE_NO_WARNINGS
)
if(SAMBAG_USE_LOG)
    add_compile_definitions(SAMBAG_USE_LOG)
endif()

# ── Dependencies ──────────────────────────────────────────────────────────────
find_package(PkgConfig REQUIRED)

# --- Boost (use Boost's own CMake config, compatible with CMake 4.x) ---
set(_BOOST_COMPONENTS
    filesystem system thread regex serialization
    date_time timer chrono program_options)
if(SAMBAG_USE_LOG)
    list(APPEND _BOOST_COMPONENTS log log_setup)
endif()
# Boost_USE_STATIC_LIBS is honoured by both the legacy FindBoost and the
# new BoostConfig approach when components are requested via find_package.
if(SAMBAG_STATIC_DEPS)
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost 1.55.0 REQUIRED COMPONENTS ${_BOOST_COMPONENTS})

# --- Cairo (via pkg-config) ---
if(SAMBAG_STATIC_DEPS)
    pkg_check_modules(Cairo REQUIRED IMPORTED_TARGET cairo)
    # Static cairo pulls in pixman, png, zlib, freetype, fontconfig …
    # The exact set depends on how cairo was compiled; pkg-config --static handles it.
    set(_CAIRO_LINK_LIBS ${Cairo_STATIC_LIBRARIES})
    set(_CAIRO_LINK_DIRS ${Cairo_STATIC_LIBRARY_DIRS})
else()
    pkg_check_modules(Cairo REQUIRED IMPORTED_TARGET cairo)
    set(_CAIRO_LINK_LIBS ${Cairo_LIBRARIES})
    set(_CAIRO_LINK_DIRS ${Cairo_LIBRARY_DIRS})
endif()

# --- Lua (5.4 preferred, 5.2 fallback) ---
pkg_check_modules(Lua lua5.4 IMPORTED_TARGET)
if(NOT Lua_FOUND)
    pkg_check_modules(Lua lua5.2 IMPORTED_TARGET)
endif()
if(NOT Lua_FOUND)
    pkg_check_modules(Lua REQUIRED lua IMPORTED_TARGET)
endif()

# SQLite3 is bundled as the amalgamation in src/sambag/cpsqlite/sqlite3.c
# (included by the custom sambag_sqlite.c). No system package needed.

# --- Loki (header-only) ---
find_path(LOKI_INCLUDE_DIR loki/Typelist.h
    PATHS /usr/include /usr/local/include
    DOC "Loki library headers (libloki-dev)")
if(NOT LOKI_INCLUDE_DIR)
    message(FATAL_ERROR
        "Loki headers not found.\n"
        "Install with:  sudo apt install libloki-dev")
endif()

# --- CppUnit (only needed for tests) ---
if(SAMBAG_BUILD_TESTS)
    pkg_check_modules(CppUnit REQUIRED cppunit IMPORTED_TARGET)
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
