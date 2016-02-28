cmake_minimum_required(VERSION 3.3)

project(mandalas CXX C)

add_subdirectory(external/SFML)

file(GLOB_RECURSE tinyfiledialogssource external/tinyfiledialogs/source/*.c)

file(GLOB_RECURSE allsource source/*.cpp)
file(GLOB mainuisource source/main-gui.cpp source/main-tui.cpp)
list(REMOVE_ITEM  allsource ${mainuisource})

execute_process(COMMAND git log -1 --date=iso "--pretty=format:%h @ %cd by %cN" OUTPUT_VARIABLE MANDALAS_VERSION)
add_definitions(-DMANDALAS_VERSION="${MANDALAS_VERSION}")

add_library(mandalas-lib STATIC ${allsource})
target_include_directories(mandalas-lib SYSTEM PUBLIC external/SFML/include)
target_include_directories(mandalas-lib SYSTEM PUBLIC external/TCLAP/include)
set_property(TARGET mandalas-lib PROPERTY CXX_STANDARD 14)
foreach(ui tui gui)
	add_library(tinyfiledialogs-${ui} STATIC ${tinyfiledialogssource})
	target_include_directories(tinyfiledialogs-${ui} PUBLIC external/tinyfiledialogs/include)

	add_executable(mandalas-${ui} source/main-${ui}.cpp)
	target_link_libraries(mandalas-${ui} mandalas-lib sfml-graphics tinyfiledialogs-${ui})
	target_include_directories(mandalas-${ui} SYSTEM PUBLIC external/boost)
	set_property(TARGET mandalas-${ui} PROPERTY CXX_STANDARD 14)
endforeach(ui)
target_compile_definitions(tinyfiledialogs-tui PUBLIC TINYFD_WIN_CONSOLE_ONLY)

set(CXX_COMPILE_FLAGS ${CXX_COMPILE_FLAGS} -Ofast -Wall -Wextra)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
