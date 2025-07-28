find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

_task(bayan OFF ""
    app.cpp
)

target_link_libraries(bayan_lib
    PRIVATE 
            Boost::system
            Boost::filesystem
            Boost::program_options
)