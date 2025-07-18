find_package(Boost REQUIRED COMPONENTS system)
find_package(SQLite3 REQUIRED)

_task(join_server OFF ""
    server.cpp
)

target_link_libraries(join_server_lib
    PRIVATE 
            Boost::system
            sqlite3
)