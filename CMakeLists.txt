cmake_minimum_required(VERSION 3.1.3)
project(Project_Template C CXX ASM)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third-party")

set(WITH_MATH 0 CACHE BOOL "Set to load math (enables some stats stuff)")
set_property(CACHE WITH_MATH
  PROPERTY STRINGS "1" "0")

set(WITH_THREAD 0 CACHE BOOL "Set to load pthread")
set_property(CACHE WITH_THREAD
  PROPERTY STRINGS "1" "0")

set(WITH_VDSO 0 CACHE BOOL "Set to include vdso (brings in -ldl)")
set_property(CACHE WITH_VDSO
  PROPERTY STRINGS "1" "0")

set(LANG "C" CACHE STRING "Set Language (CXX or C")
set_property(CACHE LANG
  PROPERTY STRINGS "C" "CXX")


set(COMPILER "gcc" CACHE STRING "Set Language (gcc or clang")
set_property(CACHE COMPILER
  PROPERTY STRINGS "clang" "gcc")


set(cxx_lang "cxx" "c++" "cpp" "cxx")
set(c_lang "c")


set(llvm_compilers "llvm" "clang" "clang++")
set(gnu_compilers "gnu" "gcc" "gxx" "g++")

string(TOLOWER ${LANG} LANG_LOWER)
string(TOLOWER ${COMPILER} COMPILER_LOWER)

set(compiler_flags_extra "")
set(link_libraries  "")

list(FIND cxx_lang ${LANG_LOWER} cxx_lang)
list(FIND c_lang ${LANG_LOWER} c_lang)
if(${cxx_lang} GREATER -1)
  set(LANG "CXX")
  set(STD "c++11")
  set(LANG_FLAGS "-fno-exceptions" "-Wno-noexcept-type" "-fno-rtti" "-Wno-ignored-attributes" "-Wstrict-aliasing=2" "-Wno-ignored-qualifiers")
elseif(${c_lang} GREATER -1)
  set(LANG "C")
  set(STD "c11")
  set(LANG_FLAGS "")
  # Pedantic C11, c++11 has too any unfortunate features
  set(LANG_FLAGS "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wdeclaration-after-statement" "-Wstrict-aliasing=1")
else()
  message(FATAL_ERROR "Invalid language selection: ${LANG}")
endif()


if(${WITH_MATH})
  list(APPEND link_libraries "-lm")
  list(APPEND compiler_flags_extra "-DWITH_MATH")
endif()

if(${WITH_THREAD})
  list(APPEND link_libraries "-lpthread")
  list(APPEND compiler_flags_extra "-DWITH_THREAD")
endif()

if(${WITH_VDSO})
  list(APPEND link_libraries "-ldl")
  list(APPEND compiler_flags_extra "-DWITH_VDSO")
endif()




list(FIND llvm_compilers ${COMPILER_LOWER} llvm_compilers)
list(FIND gnu_compilers ${COMPILER_LOWER} gnu_compilers)
if(${llvm_compilers} GREATER -1)
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  list(APPEND compiler_flags_extra "-fbracket-depth=1024" "-fmacro-backtrace-limit=0" "-Wimplicit" "-Weverything" "-Wno-gnu-flexible-array-initializer" "-Wno-gnu-statement-expression" "-Wno-disabled-macro-expansion" "-Wno-gnu-zero-variadic-macro-arguments" "-Wno-extra-semi-stmt" "-Wno-cast-align" "-Wno-missing-variable-declarations" "-Wno-extra-semi" "-Wno-padded" "-Wno-float-equal" "-Wno-covered-switch-default"  "-Wno-format-nonliteral" "-Wno-vla" "-Wno-unused-macros" "-Wno-duplicate-decl-specifier" "-Wno-double-promotion" "-Wno-unreachable-code-break" "-Wno-gnu-empty-initializer")
  if(${cxx_lang} GREATER -1)
    # Clang warns on each file which is a bit annoying. This may hide
    # real issues.
    list(APPEND compiler_flags_extra "-Wno-c++98-compat-pedantic" "-Wno-anon-enum-enum-conversion" "-Wno-c99-extensions" "-Wno-variadic-macros" "-Wno-deprecated" "-Wno-old-style-cast" "-Wno-zero-as-null-pointer-constant")
    #  else()
    #    list(APPEND compiler_flags_extra "-Wpedantic")

  endif()
elseif(${gnu_compilers} GREATER -1)
  set(CMAKE_C_COMPILER "/usr/bin/gcc")
  set(CMAKE_CXX_COMPILER "/usr/bin/g++")
  if(${c_lang} GREATER -1)
    list(APPEND compiler_flags_extra "-Wimplicit")
  endif()

else()
  message(FATAL_ERROR "Invalid compiler selection: ${COMPILER}")
endif()




set(CMAKE_EXPORT_COMPILE_COMMANDS on)


set(SRC_DIR "src")
set(FULL_SRC_DIR "${CMAKE_SOURCE_DIR}/${SRC_DIR}")

# "Bug" in cmake 3.20:
# https://discourse.cmake.org/t/base-filename-gets-set-to-compiler-depend-ts-after-update-to-cmake-3-20/3075/9
if(${CMAKE_VERSION} VERSION_LESS "3.20.0")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='\"$(subst ${FULL_SRC_DIR}/,,$(abspath $<))\"'")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${FULL_SRC_DIR}/,,$(abspath $<))\"'")
endif()

set(attrs "pure" "const" "noreturn" "format" "cold" "malloc")
set(wattrs)
foreach(attr ${attrs})
  list(APPEND wattrs "-Wsuggest-attribute=${attr}")
endforeach()

set(default_compiler_flags "-D_GNU_SOURCE" "-O2" "-Wwrite-strings" "-Wshadow" "-Wno-error=deprecated" "-Wcast-qual" "-Wcast-align" "-Winit-self"  "-Wredundant-decls" "-Wundef" "-Wswitch-enum" "-Wformat=2" "-Wconversion" "-Wdangling-else" "-Woverlength-strings" "-Wdisabled-optimization" "-Wstrict-overflow" "-Werror" "-fno-plt" "-Wall" "-Wextra" "-Wno-unused-function" "-march=native" "-Wpointer-arith" "-mtune=native" "-std=${STD}" "${LANG_FLAGS}" "${compiler_flags_extra}" "${file_name_flag}")
#"-Wc++-compat"
#"-Werror=float-equal"
set(libutil_compiler_flags ${default_compiler_flags})

set(driver_compiler_flags ${default_compiler_flags})
set(test_and_bench_compiler_flags ${default_compiler_flags})
set(test_and_bench_libraries ${link_libraries})

list(APPEND test_and_bench_compiler_flags "-DWITH_THREAD" "-DWITH_MATH" "-DWITH_VDSO")
list(APPEND test_and_bench_libraries "-lpthread" "-lm" "-ldl")


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third-party")
file(GLOB_RECURSE C_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/*.c")
set_source_files_properties(${C_SOURCES} PROPERTIES LANGUAGE ${LANG})

set(ALL_C_SOURCES ${C_SOURCES})

file(GLOB_RECURSE ASM_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/*.S")
list(FILTER ASM_SOURCES EXCLUDE REGEX ".*/core/.*\.S$")

# Filter test sources
list(FILTER C_SOURCES EXCLUDE REGEX ".*/test/.*$")
list(FILTER C_SOURCES EXCLUDE REGEX ".*\.test\.c$")

# Filter bench sources
list(FILTER C_SOURCES EXCLUDE REGEX ".*/bench/.*$")
list(FILTER C_SOURCES EXCLUDE REGEX ".*\.bench\.c$")


# Setup driver executable
set(DRIVER_SRC ${SRC_DIR}/driver.c)
list(REMOVE_ITEM C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${DRIVER_SRC}")

# Setup test executable
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/*.test.c")
file(GLOB_RECURSE TEST_UTIL_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/test/*.c")
set(TEST_C_SOURCES ${TEST_SOURCES} ${TEST_UTIL_SOURCES})


# Setup bench executable
file(GLOB_RECURSE BENCH_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/*.bench.c")
file(GLOB_RECURSE BENCH_UTIL_SOURCES "${CMAKE_SOURCE_DIR}/${SRC_DIR}/bench/*.c")
set(BENCH_C_SOURCES ${BENCH_SOURCES} ${BENCH_UTIL_SOURCES})




find_program(CLANG_TIDY_EXE clang-tidy NAMES clang-tidy-14 clang-tidy-13 clang-tidy-12 clang-tidy-10 clang-tidy-9 clang-tidy-8)
# We don't get cache with this method which slows down check.
add_custom_target(
  static-analysis
  COMMAND ${CLANG_TIDY_EXE}
  ${ALL_C_SOURCES}
  -p=${CMAKE_CURRENT_BINARY_DIR}
)

add_library(util STATIC ${C_SOURCES} ${ASM_SOURCES})
target_compile_options(util PRIVATE ${libutil_compiler_flags})
set_target_properties(util PROPERTIES LINKER_LANGUAGE ${LANG})

add_executable(driver ${DRIVER_SRC})
target_compile_options(driver PRIVATE ${driver_compiler_flags})
target_link_libraries(driver util ${link_libraries})
set_target_properties(driver PROPERTIES LINKER_LANGUAGE ${LANG})

add_executable(tests ${TEST_C_SOURCES})
target_compile_options(tests PRIVATE ${test_and_bench_compiler_flags})
target_link_libraries(tests util ${test_and_bench_libraries})
set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
set_target_properties(tests PROPERTIES LINKER_LANGUAGE ${LANG})

add_executable(bench ${BENCH_C_SOURCES})
target_compile_options(bench PRIVATE ${test_and_bench_compiler_flags})
target_link_libraries(bench util ${test_and_bench_libraries})
set_target_properties(bench PROPERTIES EXCLUDE_FROM_ALL TRUE)
set_target_properties(bench PROPERTIES LINKER_LANGUAGE ${LANG})

add_custom_target(full)
add_dependencies(full util driver tests bench)


#Setup sysinfo executable
#set(SYS_C_SOURCES ${C_SOURCES})
#list(FILTER C_SOURCES EXCLUDE REGEX ".*/sysinfo/.*$")
#set(SYS_EXE_SRC "${SRC_DIR}/sysinfo/sysinfo-driver.c")
#list(REMOVE_ITEM SYS_C_SOURCES ${SYS_EXE_SRC})
#add_executable(sysinfo_builder ${SYS_EXE_SRC} ${SYS_C_SOURCES})
#target_compile_options(sysinfo_builder PRIVATE ${driver_compiler_flags})
#target_link_libraries(sysinfo_builder ${link_libraries} "cpu_features")
#set_target_properties(sysinfo_builder PROPERTIES EXCLUDE_FROM_ALL TRUE)
#set_target_properties(sysinfo_builder PROPERTIES LINKER_LANGUAGE ${LANG})
#target_include_directories(sysinfo_builder PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party")
#add_custom_target(sysinfo sysinfo_builder cpu_features)
