link_libraries(socketv)
include_directories("${SOCKETV_SOURCE_DIR}/include")

aux_source_directory("src" SOCKETV_EXAMPLE_UDP_CLIENT_ALL_SOURCE)

add_executable(udp_client ${SOCKETV_EXAMPLE_UDP_CLIENT_ALL_SOURCE})