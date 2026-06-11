# ══════════════════════════════════════════════════════════════════════════════
#  CoroWeaver (header-only INTERFACE library)
# ══════════════════════════════════════════════════════════════════════════════

set(INC_CONCURRENTQUEUE "${CMAKE_SOURCE_DIR}/vendor/concurrentqueue")

add_library(CoroWeaver INTERFACE)
add_library(CoroWeaver::CoroWeaver ALIAS CoroWeaver)

file(GLOB_RECURSE COROWEAVER_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

target_compile_features(CoroWeaver INTERFACE cxx_std_20)

target_sources(CoroWeaver INTERFACE
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src"
    FILES ${COROWEAVER_HEADERS}
)

target_include_directories(CoroWeaver
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        "${INC_CONCURRENTQUEUE}"
)

target_compile_definitions(CoroWeaver
    INTERFACE
        # Config-specific defines via generator expressions
        $<$<CONFIG:Debug>:CW_DEBUG>
        $<$<CONFIG:Release>:CW_RELEASE>
)


