cmake_minimum_required(VERSION 3.3)

project(mandalas CXX)

add_subdirectory(external/SFML)

set(BUILD_SHARED_LIBS FALSE CACHE BOOL "")

file(GLOB_RECURSE source source/*.cpp)
add_executable(mandalas ${source})
target_link_libraries(mandalas sfml-graphics)
target_include_directories(mandalas SYSTEM PUBLIC external/SFML/include)

set(CXX_COMPILE_FLAGS ${CXX_COMPILE_FLAGS} -O3 -Wall -Wextra)
set_property(TARGET mandalas PROPERTY CXX_STANDARD 14)
