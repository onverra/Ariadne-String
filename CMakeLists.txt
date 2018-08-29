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

if (MSVC)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Ariadne-String)
endif ()

add_library(pugixml STATIC ${CMAKE_SOURCE_DIR}/3rdparty/pugixml/src/pugixml.cpp)
set_target_properties(pugixml PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)

# Set Local Debugger Settings (Command Arguments and Environment Variables)
# Credits: https://gist.github.com/UnaNancyOwen/665b3d7993785749b1033217d73540fa

set( COMMAND_ARGUMENTS "${CMAKE_SOURCE_DIR}/doc/hitchcock.jpg" )
set( ENVIRONMENT_VARIABLES ${CMAKE_SOURCE_DIR}/${OpenCV_DIR}bin/${CMAKE_CFG_INTDIR} )
configure_file( bat/template.vcxproj.user.in ${CMAKE_BINARY_DIR}/src/Ariadne-String.vcxproj.user @ONLY )
