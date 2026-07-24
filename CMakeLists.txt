cmake_minimum_required(VERSION 3.28)

project("Pinguin OS" ASM C CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(ARCH=${ARCH})
add_compile_definitions(ARCH_${ARCH})

if("${TARGET}" STREQUAL "TESTS")
  add_compile_definitions(TESTING)
endif()

if(NOT "${TARGET}" STREQUAL "TESTS")
  if("${ARCH}" STREQUAL "x86")
    add_subdirectory(boot/stage1)
    add_subdirectory(boot/stage2)
  endif()

  add_subdirectory(core_lib)
  add_subdirectory(kernel)
else()
  add_subdirectory(core_lib)
  add_subdirectory(tests)
endif()
