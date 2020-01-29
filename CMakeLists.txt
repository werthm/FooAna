cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(FooAna)

# more warnings
add_definitions(-D_FORTIFY_SOURCE=2)
add_definitions(-Wall)

# set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Mac OS X special settings
if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif(APPLE)

# find ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Tree EG)
include(${ROOT_USE_FILE})

# source file globbing
file(GLOB SRCS src/FA*.cxx)

# header directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# create the dictionary
file(GLOB HDR ${CMAKE_CURRENT_SOURCE_DIR}/include/FA*.h)
foreach(head_file ${HDR})
    # extract pure header file name
    get_filename_component(pure_hdr ${head_file} NAME)
    set(HEADERS ${HEADERS} ${pure_hdr})
endforeach()

add_custom_command(OUTPUT G__FooAna.cxx
    COMMAND rootcling -f G__FooAna.cxx
                      -rml libFooAna.so
                      -rmf libFooAna.rootmap
                      -s libFooAna.so
                      -I${CMAKE_CURRENT_SOURCE_DIR}/include
                      ${HEADERS}
                      ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkDef.h
)

# create the shared library
add_library(FooAna SHARED ${SRCS} G__FooAna.cxx)
target_link_libraries(FooAna ${ROOT_LIBRARIES})

# installation
install(TARGETS FooAna LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/lib)
install(FILES ${PROJECT_BINARY_DIR}/libFooAna.rootmap DESTINATION ${PROJECT_BINARY_DIR}/lib)
install(FILES ${PROJECT_BINARY_DIR}/libFooAna_rdict.pcm DESTINATION ${PROJECT_BINARY_DIR}/lib)
install(FILES ${HDR} DESTINATION ${PROJECT_BINARY_DIR}/include)

