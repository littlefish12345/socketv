link_libraries(socketv)
include_directories("${SOCKETV_SOURCE_DIR}/include")

aux_source_directory("src" SOCKETV_EXAMPLE_UDP_SERVER_ALL_SOURCE)

add_executable(udp_server ${SOCKETV_EXAMPLE_UDP_SERVER_ALL_SOURCE})