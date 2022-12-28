# CMakeList.txt : CMake project for App, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("App")

# Add source to this project's executable.
add_executable (App "App.cpp" "App.h" "Archiver.hpp" "Compressor.hpp" "Compressor.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET App PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
