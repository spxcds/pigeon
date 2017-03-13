cmake_minimum_required (VERSION 2.8.4)

file (GLOB_RECURSE ALL_SRC_FILES *.c)
file (GLOB_RECURSE ALL_INCLUDE_FILES *.h)

add_definitions (-D_GNU_SOURCE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Werror=implicit-function-declaration") 

add_library (thpool STATIC ${ALL_SRC_FILES})
install(TARGETS thpool ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)
