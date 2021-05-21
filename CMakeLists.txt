PROJECT(jpdaf_tracker)

SET(execName jpdaf_tracker)

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 -g")

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include(CheckIncludeFileCXX)
check_include_file_cxx(filesystem HAVE_FILESYSTEM)
if(HAVE_FILESYSTEM)
    add_definitions(-DHAVE_FILESYSTEM)
    message("Founded filesystem header")
else(HAVE_FILESYSTEM)
    message("Do not found filesystem header")
endif(HAVE_FILESYSTEM)

include_directories( src )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${EIGEN3_INCLUDE_DIR})

file(GLOB_RECURSE tracker_SRC "src/*.cpp")
file(GLOB_RECURSE tracker_INC "src/*.h")
add_executable(${execName} ${tracker_SRC} ${tracker_INC})

TARGET_LINK_LIBRARIES( ${execName} ${OpenCV_LIBS} stdc++fs)
