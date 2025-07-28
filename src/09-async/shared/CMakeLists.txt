add_library(async_shared SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/async.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread-listener.cpp
)
target_include_directories(async_shared
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
set_target_properties(async_shared PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME async
)
target_link_libraries(async_shared 
    PUBLIC
        bulk_i
)
target_compile_features(async_shared PUBLIC cxx_std_20)
install(TARGETS async_shared DESTINATION lib COMPONENT async)