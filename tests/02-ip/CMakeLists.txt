set(TEST_DATA_ROOT "${CMAKE_SOURCE_DIR}/data/")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/roots.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/roots.h
)

find_package(OpenSSL REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIRS})

_test(test_ip_filter test_ip_filter.cpp ip_filter_lib ${OPENSSL_LIBRARIES})