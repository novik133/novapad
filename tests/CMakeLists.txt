##############################################################################
# NovaPad - Test Suite CMake Configuration
#
# Uses Qt Test framework for unit and integration tests.
# Run with: ctest --test-dir build
##############################################################################

cmake_minimum_required(VERSION 3.25)

find_package(Qt6 REQUIRED COMPONENTS Test)

enable_testing()

# ---------------------------------------------------------------------------
# Test helper: creates a test executable and registers it with CTest
# ---------------------------------------------------------------------------
function(novapad_add_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE
        novapad_lib
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test
        Qt6::PrintSupport
        Qt6::Network
        Qt6::Svg
        Qt6::SvgWidgets
        Qt6::Core5Compat
    )
    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}/generated
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# ---------------------------------------------------------------------------
# Test targets
# ---------------------------------------------------------------------------
novapad_add_test(test_text_operations  test_text_operations.cpp)
novapad_add_test(test_encoding_detector test_encoding_detector.cpp)
novapad_add_test(test_search_helpers   test_search_helpers.cpp)
novapad_add_test(test_language_manager test_language_manager.cpp)
novapad_add_test(test_color_scheme     test_color_scheme.cpp)
novapad_add_test(test_persistent_cache test_persistent_cache.cpp)
