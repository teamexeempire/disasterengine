cmake_minimum_required(VERSION 3.8)
project(modpack)

# Test for endianess
include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)

if(IS_BIG_ENDIAN)
 message(STATUS "System is Big-Endian")
 set(BIG_ENDIAN true)
else()
 message(STATUS "System is Little-Endian")
 set(BIG_ENDIAN false)
endif()

configure_file(config.h.in config.h @ONLY)
add_executable(modpack "Main.cc")
target_include_directories(modpack PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET modpack PROPERTY CXX_STANDARD 20)
endif()