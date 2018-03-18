cmake_minimum_required(VERSION 2.6)
project (critbit C)
include_directories (../cutest)

FILE (GLOB LIB_HDR *.h)
add_library (${PROJECT_NAME} critbit_clear.c critbit_erase.c critbit_find.c critbit_find_prefix.c critbit_foreach.c critbit_get_kv.c critbit_insert.c critbit_new_kv.c ${LIB_HDR})

add_executable (benchmark benchmark.c strtolh.c)
target_link_libraries (benchmark critbit)

add_executable (test_critbit cb_tests.c test_critbit.c)
target_link_libraries (test_critbit critbit cutest)

enable_testing()
add_test (critbit test_critbit)

IF (MSVC)
    find_package (MSVC MODULE)
    MSVC_CRT_SECURE_NO_WARNINGS (benchmark)
ENDIF (MSVC)

set (CRITBIT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "CritBit headers")
set (CRITBIT_LIBRARIES critbit CACHE INTERNAL "CritBit libraries")
