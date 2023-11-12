cmake_minimum_required(VERSION 3.15)

project(socketv C CXX)

option(BUILD_SHARED "build shared library" OFF)
option(BUILD_EXAMPLE "build example program" ON)

if(WIN32)
    aux_source_directory("${PROJECT_SOURCE_DIR}/src/windows" SOCKETV_ALL_SOURCE)
elseif(UNIX AND NOT APPLE)
    aux_source_directory("${PROJECT_SOURCE_DIR}/src/unix" SOCKETV_ALL_SOURCE)
elseif(APPLE)
    aux_source_directory("${PROJECT_SOURCE_DIR}/src/apple" SOCKETV_ALL_SOURCE)
endif()

if(BUILD_SHARED)
    add_library(socketv SHARED ${SOCKETV_ALL_SOURCE})
else()
    add_library(socketv STATIC ${SOCKETV_ALL_SOURCE})
endif()

if(WIN32)
    target_include_directories(socketv PRIVATE "${PROJECT_SOURCE_DIR}/src/windows/include")
    target_compile_definitions(socketv PRIVATE WIN)
    target_link_libraries(socketv ws2_32)
elseif(UNIX AND NOT APPLE)
    target_include_directories(socketv PRIVATE "${PROJECT_SOURCE_DIR}/src/unix/include")
    target_compile_definitions(socketv PRIVATE UNIX)
elseif(APPLE)
    target_include_directories(socketv PRIVATE "${PROJECT_SOURCE_DIR}/src/apple/include")
    target_compile_definitions(socketv PRIVATE APPLE)
endif()

if(BUILD_EXAMPLE)
    add_subdirectory("example")
endif()

target_include_directories(socketv PUBLIC "${PROJECT_SOURCE_DIR}/include")