cmake_minimum_required(VERSION 3.30.0)

project(viewer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(wxWidgets REQUIRED COMPONENTS core base)

include(${wxWidgets_USE_FILE})

add_subdirectory(src)

  