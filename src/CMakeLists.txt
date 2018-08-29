file(GLOB Ariadne-String_CPP
    "*.cpp"
  )


set(Ariadne-String_SRC
  ${Ariadne-String_CPP}
  ../3rdparty/imgui/examples/libs/gl3w/GL/gl3w.c
  ../3rdparty/imgui/imgui_draw.cpp
  ../3rdparty/imgui/imgui.cpp
  ../3rdparty/imgui/examples/imgui_impl_glfw.cpp
  ../3rdparty/imgui/examples/imgui_impl_opengl3.cpp
  )

set(Ariadne-String_HDR
  AlgSimple.h
  Preview.h
  Solution.h
  )

file(GLOB Ariadne-String_HDR
    "*.h"
  )


include_directories(${CMAKE_SOURCE_DIR}/3rdparty/imgui/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/glfw/include/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/imgui/examples/libs/gl3w)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/imgui/examples)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/pugixml/src)

link_directories(${CMAKE_SOURCE_DIR}/3rdparty/glfw/build/src/$(ConfigurationName))

add_executable(Ariadne-String ${Ariadne-String_SRC} ${Ariadne-String_HDR})

target_link_libraries(Ariadne-String ${OPENGL_LIBRARIES} ${OpenCV_LIBS} pugixml glfw3)
