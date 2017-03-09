cmake_minimum_required (VERSION 2.8.4)

file (GLOB_RECURSE ALL_SRC_FILES *.c)
file (GLOB_RECURSE ALL_INCLUDE_FILES *.h)

add_definitions (-D_GNU_SOURCE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Werror=implicit-function-declaration") 

link_directories(${PROJECT_SOURCE_DIR}/lib)
list(APPEND LIBS "thpool" "pthread")
add_executable (server ${ALL_SRC_FILES})
target_link_libraries(server ${LIBS})