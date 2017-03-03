cmake_minimum_required (VERSION 2.8.4)

file (GLOB_RECURSE ALL_SRC_FILES *.c)
file (GLOB_RECURSE ALL_INCLUDE_FILES *.h)

message (${ALL_SRC_FILES})
add_executable (server ${ALL_SRC_FILES})
