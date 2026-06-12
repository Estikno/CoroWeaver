# ══════════════════════════════════════════════════════════════════════════════
#  Tests
# ══════════════════════════════════════════════════════════════════════════════

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp")

add_executable(tests ${TEST_SOURCES})

target_include_directories(tests PRIVATE
    "${CMAKE_SOURCE_DIR}/vendor/doctest/doctest"
    "${CMAKE_SOURCE_DIR}/vendor/concurrentqueue"
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(tests PRIVATE CoroWeaver::CoroWeaver)

target_compile_definitions(tests PRIVATE
    $<$<CONFIG:Debug>:TEST_DEBUG>
)

set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BASE}/$<CONFIG>/${CMAKE_SYSTEM_NAME}-x64/Tests"
)

target_compile_definitions(tests PRIVATE
    CW_TESTING
    $<$<CONFIG:Debug>:CW_DEBUG>
)

set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BASE}/$<CONFIG>/${CMAKE_SYSTEM_NAME}-x64/Tests"
)

# For multi-config generators (Visual Studio): exclude from Release & Dist
set_target_properties(tests PROPERTIES
    EXCLUDE_FROM_ALL $<OR:$<CONFIG:Release>,$<CONFIG:Dist>>
)
