link_libraries(socketv)
include_directories("${SOCKETV_SOURCE_DIR}/include")

aux_source_directory("src" SOCKETV_EXAMPLE_DEV_TEST_ALL_SOURCE)

add_executable(dev_test ${SOCKETV_EXAMPLE_DEV_TEST_ALL_SOURCE})