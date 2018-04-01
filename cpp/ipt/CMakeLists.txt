# Minimum version
cmake_minimum_required(VERSION 3.6)
project(ipt)

# create share lib
file(GLOB IP_TABLES_SRC_FILES "*.cpp")
list(REMOVE_ITEM IP_TABLES_SRC_FILES main.cpp)
add_library(ip_tables_lib SHARED ${IP_TABLES_SRC_FILES})

# create the executable
add_executable(ip_tables main.cpp)

target_link_libraries(ip_tables ip_tables_lib)
