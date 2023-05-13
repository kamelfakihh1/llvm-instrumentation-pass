# set(LT_LLVM_INSTALL_DIR "" CACHE PATH "LLVM installation directory")
# set(LT_LLVM_INCLUDE_DIR "${LT_LLVM_INSTALL_DIR}/include/llvm")

# add_library(InstrumentFunction MODULE src/InstrumentFunction.cpp)
# add_library(MyFunction STATIC src/myFunction.cpp)

# # Configure include directories for 'plugin'
# include_directories(
#   "${CMAKE_CURRENT_SOURCE_DIR}/include"
# )

# set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
# set(CMAKE_CXX_FLAGS "-Wall -fno-rtti -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -fPIC -fdiagnostics-color=always")

cmake_minimum_required(VERSION 3.13.4)
project(llvm-instrument-function)

#===============================================================================
# 1. LOAD LLVM CONFIGURATION
#===============================================================================
# Set this to a valid LLVM installation dir
set(LT_LLVM_INSTALL_DIR "" CACHE PATH "LLVM installation directory")

# Add the location of LLVMConfig.cmake to CMake search paths (so that
# find_package can locate it)
list(APPEND CMAKE_PREFIX_PATH "${LT_LLVM_INSTALL_DIR}/lib/cmake/llvm/")

# FIXME: This is a warkaround for #25. Remove once resolved and use
find_package(LLVM 17 REQUIRED CONFIG)

# HelloWorld includes headers from LLVM - update the include paths accordingly
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

# Configure include directories for 'plugin'
include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

#===============================================================================
# 2. LLVM-TUTOR BUILD CONFIGURATION
#===============================================================================
# Use the same C++ standard as LLVM does
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-rtti -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -fPIC -fdiagnostics-color=always")

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()


add_library(InstrumentFunction MODULE src/InstrumentFunction.cpp)
add_library(MyFunction STATIC src/myFunction.cpp)

# Allow undefined symbols in shared objects on Darwin (this is the default
# behaviour on Linux)
target_link_libraries(InstrumentFunction
  "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>")