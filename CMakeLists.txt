cmake_minimum_required(VERSION 3.15)

project(socketv C CXX)

option(BUILD_SHARED "build shared library" OFF)
option(BUILD_EXAMPLE "build example program" ON)

aux_source_directory("${PROJECT_SOURCE_DIR}/src" SOCKETV_ALL_SOURCE)

if(BUILD_SHARED)
    add_library(socketv SHARED ${SOCKETV_ALL_SOURCE})
else()
    add_library(socketv STATIC ${SOCKETV_ALL_SOURCE})
endif()

target_include_directories(socketv PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(socketv PRIVATE "${PROJECT_SOURCE_DIR}/src/include")

if(WIN32)
    target_compile_definitions(socketv PRIVATE WINDOWS)
    target_link_libraries(socketv ws2_32)
elseif(UNIX)
    target_compile_definitions(socketv PRIVATE UNIX)
endif()

if(BUILD_EXAMPLE)
    add_subdirectory("example")
endif()