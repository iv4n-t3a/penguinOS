include(FetchContent)

set(TEST_SOURCES
    kernel/mem/buddy_test.cpp kernel/mem/buddy_util_test.cpp
    kernel/mem/freelist_test.cpp kernel/mem/kmalloc_tests.cpp
    kernel/mem/slab_test.cpp kernel/mem/util_test.cpp)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/kernel/arch/debug/vga.c
    ${PROJECT_SOURCE_DIR}/kernel/mem/buddy.c
    ${PROJECT_SOURCE_DIR}/kernel/mem/buddy_util.c
    ${PROJECT_SOURCE_DIR}/kernel/mem/freelist.c
    ${PROJECT_SOURCE_DIR}/kernel/mem/kmalloc.c
    ${PROJECT_SOURCE_DIR}/kernel/mem/pfalloc.c
    ${PROJECT_SOURCE_DIR}/kernel/mem/slab.c
    ${PROJECT_SOURCE_DIR}/kernel/mem/util.c
    ${PROJECT_SOURCE_DIR}/kernel/libs/kernel_log.c
    ${PROJECT_SOURCE_DIR}/kernel/libs/stdio.c
    kernel/mem/mem_test_util.cpp)

set(HEADERS
    ${PROJECT_SOURCE_DIR}/kernel/mem/buddy.h
    ${PROJECT_SOURCE_DIR}/kernel/mem/buddy_util.h
    ${PROJECT_SOURCE_DIR}/kernel/mem/freelist.h
    ${PROJECT_SOURCE_DIR}/kernel/mem/kmalloc.h
    ${PROJECT_SOURCE_DIR}/kernel/mem/pfalloc.h
    ${PROJECT_SOURCE_DIR}/kernel/mem/slab.h
    ${PROJECT_SOURCE_DIR}/kernel/mem/util.h
    ${PROJECT_SOURCE_DIR}/kernel/libs/kernel_log.h
    ${PROJECT_SOURCE_DIR}/kernel/libs/kernel_panic.h
    ${PROJECT_SOURCE_DIR}/kernel/libs/stdio.h
    kernel/mem/mem_test_util.h)

add_executable(unit_tests ${TEST_SOURCES} ${SOURCES} ${HEADERS})

target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(unit_tests core_lib)

# Google tests setup:

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
target_link_libraries(unit_tests GTest::gtest_main)
gtest_discover_tests(unit_tests)
