cmake_minimum_required(VERSION 3.1.0)

project(Ariadne-String)

if(MSVC)
  set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
endif()

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

option(ENABLE_SOLUTION_FOLDERS "Solution folder in Visual Studio or in other IDEs" MSVC_IDE)

if(ENABLE_SOLUTION_FOLDERS)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endif()

add_definitions(-std=c++14)
add_definitions(-D_USE_MATH_DEFINES)


add_library(pugixml STATIC ${CMAKE_SOURCE_DIR}/3rdparty/pugixml/src/pugixml.cpp)
set_target_properties(pugixml PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)

