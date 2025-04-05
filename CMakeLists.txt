cmake_minimum_required(VERSION 3.30)

project(Caravan)

set(CMAKE_CXX_STANDARD 23)

# Disable security warnings for unsafe C functions in VST3 SDK
# This is needed because the JUCE VST3 implementation uses functions 
# like strcpy, sprintf, etc. that MSVC flags as unsafe
if(MSVC)
    # Add this globally to silence the warnings in VST3 SDK
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    
    # Disable treating warnings as errors for JUCE code only
    # but keep it for our own code
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include(cmake/cpm.cmake)

CPMAddPackage(
  NAME JUCE
  GITHUB_REPOSITORY juce-framework/JUCE
  GIT_TAG 8.0.6
  VERSION 8.0.6
  SOURCE_DIR ${LIB_DIR}/juce
)

# Tell JUCE to enable OpenGL module
set(JUCE_ENABLE_MODULE_SOURCE_GROUPS ON)
option(JUCE_OPENGL_INCLUDE_MODERN_HEADERS "Include OpenGL headers needed for OpenGL 3.2 and above" ON)

# Add the plugin directory
add_subdirectory(plugin)