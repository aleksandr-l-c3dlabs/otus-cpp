find_package(Boost REQUIRED COMPONENTS system)

_task(bulk_server OFF ""
    server.cpp
)

target_link_libraries(bulk_server_lib
    PUBLIC
    
    PRIVATE 
        async_shared
        Boost::system
)