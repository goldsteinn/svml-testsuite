cmake_minimum_required(VERSION 3.1.3)
project(MATMUL C ASM)


set(CMAKE_EXPORT_COMPILE_COMMANDS on)


set(SRC_DIR "src")
set(FULL_SRC_DIR "${CMAKE_SOURCE_DIR}/${SRC_DIR}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='\"$(subst ${FULL_SRC_DIR}/,,$(abspath $<))\"'")

add_compile_options(-D_GNU_SOURCE -O2 -Wall -Wextra -Wno-unused-function -Wno-variadic-macros -Wno-gnu-zero-variadic-macro-arguments -march=native -mtune=native -std=gnu11)


set(EXE_SRC ${SRC_DIR}/driver.c)
set(EXE "driver")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}")
file(GLOB_RECURSE C_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/*.c")
list(REMOVE_ITEM C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${EXE_SRC}")

set (EXCLUDE_DIR "/core/")
file(GLOB_RECURSE ASM_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/*.S")
list(FILTER ASM_SOURCES EXCLUDE REGEX ".*/core/.*\.S$")

list(FILTER C_SOURCES EXCLUDE REGEX ".*/test/.*$")
list(FILTER C_SOURCES EXCLUDE REGEX ".*\.test\.c$")

# Setup test executable
set(TEST_EXE_SRC "${SRC_DIR}/test/test-driver.c")
set(TEST_EXE "test-driver")
file(GLOB_RECURSE TEST_C_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/*.test.c")
list(REMOVE_ITEM TEST_C_SOURCES ${TEST_EXE_SRC})


find_program(CLANG_TIDY_EXE clang-tidy NAMES clang-tidy-8 clang-tidy-9 clang-tidy-10 clang-tidy-11 clang-tidy-12 clang-tidy-13)
# We don't get cache with this method which slows down check.
add_custom_target(
  static-analysis
  COMMAND ${CLANG_TIDY_EXE}
  ${C_SOURCES}
  -p=${CMAKE_CURRENT_BINARY_DIR}
  -checks=-*insecureAPI.strcpy,-*insecureAPI.DeprecatedOrUnsafeBufferHandling
)

add_executable(driver ${EXE_SRC} ${C_SOURCES} ${ASM_SOURCES})
target_link_libraries(driver)


add_executable(tests ${TEST_EXE_SRC} ${TEST_C_SOURCES} ${C_SOURCES})
target_link_libraries(tests)
set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
