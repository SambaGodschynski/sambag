# Collect all C/C++ sources under src/sambag/
file(GLOB_RECURSE _ALL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/sambag/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sambag/*.c"
)

# ── Exclusion filters ─────────────────────────────────────────────────────────

# sqlite3.c is included via #include inside sambag_sqlite.c — not a direct TU
list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/cpsqlite/sqlite3\\.c$")

# Test files and test applications
list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/[Tt]ests?/[^/]+\\.cpp$")
list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*[Tt]est[A-Z][^/]*\\.cpp$")
list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/testApps/.*\\.cpp$")

# macOS-only files (Cocoa / Quartz / CoreGraphics)
if(NOT APPLE)
    list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/[Cc]ocoa[^/]*\\.cpp$")
    list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/cocoaimpl/.*\\.cpp$")
    list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*[Qq]uartz[^/]*\\.cpp$")
    list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/cairoCG[^/]*\\.cpp$")
    # cairoCreateImageSurfaceDelegate.cpp is cross-platform (guarded internally),
    # only exclude the macOS-only Quartz context wrapper above.
endif()

# Windows-only files
if(NOT WIN32)
    list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*[Ww]in32[^/]*\\.cpp$")
endif()

# VST-specific DSP sources (need VST2/VST3 SDK headers)
if(NOT SAMBAG_VST_SUPPORT)
    list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/dsp/TimeInfoVst[^/]+\\.cpp$")
    list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/dsp/Vst[^/]+\\.cpp$")
    list(FILTER _ALL_SOURCES EXCLUDE REGEX ".*/dsp/VstMidiEventAdapter\\.cpp$")
endif()

# ── Library target ────────────────────────────────────────────────────────────
add_library(sambag STATIC ${_ALL_SOURCES})

target_include_directories(sambag
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"          # so #include <sambag/...> works
        "${PROJECT_BINARY_DIR}/src"            # generated Config.h
        ${Boost_INCLUDE_DIRS}
        ${LOKI_INCLUDE_DIR}
)

# Boost modern targets
set(_BOOST_TARGETS
    Boost::filesystem Boost::system Boost::thread Boost::regex
    Boost::serialization Boost::date_time Boost::timer
    Boost::chrono Boost::program_options)
if(SAMBAG_USE_LOG)
    list(APPEND _BOOST_TARGETS Boost::log Boost::log_setup)
endif()

target_link_libraries(sambag
    PUBLIC
        ${_BOOST_TARGETS}
        ${SAMBAG_CAIRO_TARGET}
        ${SAMBAG_LUA_TARGET}
        ticpp_ticpp
)

if(UNIX AND NOT APPLE)
    target_link_libraries(sambag PUBLIC ${X11_LIBRARIES} rt pthread dl)
    target_include_directories(sambag PUBLIC ${X11_INCLUDE_DIRS})
elseif(APPLE)
    target_link_libraries(sambag PUBLIC ${COCOA_LIBRARY} ${CARBON_LIBRARY})
endif()

if(SAMBAG_VST_SUPPORT)
    target_include_directories(sambag PUBLIC
        ${VSTSDK_INCLUDE_DIR}
        ${VSTSDK_INCLUDE_DIR}/public.sdk/source/vst2.x
        ${VSTSDK_INCLUDE_DIR}/pluginterfaces/vst2.x
        ${VSTSDK3_INCLUDE_DIR}
        ${VSTSDK3_INCLUDE_DIR}/public.sdk/source/vst
        ${VSTSDK3_INCLUDE_DIR}/pluginterfaces/vst
    )
endif()

# ── Unit tests ────────────────────────────────────────────────────────────────
if(SAMBAG_BUILD_TESTS)
    # Collect CppUnit test sources from all modules
    file(GLOB_RECURSE _TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/sambag/*/tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/sambag/*/tests/**/*.cpp"
    )

    # Exclude test helper that is a standalone executable, not a test suite
    list(FILTER _TEST_SOURCES EXCLUDE REGEX ".*/test_shm_counterpart\\.cpp$")

    # Exclude Win32-specific tests on non-Windows
    if(NOT WIN32)
        list(FILTER _TEST_SOURCES EXCLUDE REGEX ".*[Ww]in32[^/]*\\.cpp$")
    endif()

    # Exclude VST-dependent tests when VST support is off
    if(NOT SAMBAG_VST_SUPPORT)
        list(FILTER _TEST_SOURCES EXCLUDE REGEX ".*/dsp/tests/TestVst[^/]+\\.cpp$")
        list(FILTER _TEST_SOURCES EXCLUDE REGEX ".*/dsp/tests/TestTimeInfoVst[^/]+\\.cpp$")
        list(FILTER _TEST_SOURCES EXCLUDE REGEX ".*/dsp/tests/TestDefaultMidiEvents\\.cpp$")
    endif()

    add_executable(unit_tests
        "${CMAKE_CURRENT_SOURCE_DIR}/testApps/sambag_tests.cpp"
        ${_TEST_SOURCES}
    )
    target_link_libraries(unit_tests PRIVATE sambag ${SAMBAG_CPPUNIT_TARGET})

    # test_shm_counterpart (32/64-bit SHM bridge prototype) excluded — unfinished

    enable_testing()
    add_test(NAME sambag_unit_tests COMMAND unit_tests)
endif()

# ── UI test applications ──────────────────────────────────────────────────────
if(SAMBAG_BUILD_TESTAPP)
    # TestApp is the only UI test that was part of the original CMake build
    # (cmakeWalker.py → SAMBAG_TESTAPPSOURCES).
    # TestButtons / TestRootPane / Calculator were Eclipse CDT projects only.
    set(_TESTAPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/testApps/componentTests")

    add_executable(testApp
        "${_TESTAPP_DIR}/TestApp/src/DiscoView.cpp"
    )
    target_link_libraries(testApp PRIVATE sambag)
endif()
