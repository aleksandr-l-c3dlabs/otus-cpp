find_package(Eigen3 3.3 REQUIRED NO_MODULE)

_task(fashio_mnist OFF "")

target_link_libraries(join_server_lib
    PRIVATE 
            Eigen3::Eigen
)