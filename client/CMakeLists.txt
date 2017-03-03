cmake_minimum_required (VERSION 2.8.4)

file (GLOB_RECURSE ALL_SRC_FILES *.c *.cpp *.cc)
file (GLOB_RECURSE ALL_INCLUDE_FILES *.h)

add_executable (client ${ALL_SRC_FILES})