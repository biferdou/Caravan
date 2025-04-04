cmake_minimum_required(VERSION 3.30)

project(Caravan)

set(CMAKE_CXX_STANDARD 23)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include(cmake/cpm.cmake)

CPMAddPackage(
  NAME JUCE
  GITHUB_REPOSITORY juce-framework/JUCE
  GIT_TAG 8.0.6
  VERSION 8.0.6
  SOURCE_DIR ${LIB_DIR}/juce
)