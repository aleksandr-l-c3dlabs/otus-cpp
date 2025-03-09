configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

# include_directories(${CMAKE_CURRENT_BINARY_DIR})

_task(helloworld)