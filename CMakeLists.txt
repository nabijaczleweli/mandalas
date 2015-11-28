project(mandalas CXX)

set(CXX_COMPILE_FLAGS ${CXX_COMPILE_FLAGS} -O3 -Wall -Wextra)

file(GLOB_RECURSE source source/*.cpp)
add_executable(${source})

set_property(TARGET mandalas PROPERTY CXX_STANDARD 14)
