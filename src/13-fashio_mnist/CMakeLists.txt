find_package(Eigen3 3.3 REQUIRED NO_MODULE)

_task(fashio_mnist OFF "")

target_link_libraries(join_server_lib
    PRIVATE 
            Eigen3::Eigen
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(
        FILES 
            "./preset/fashion-mnist_train.csv"
            "./preset/logreg_coef.txt"
        DESTINATION "/var/lib/fashio_mnist"
        PERMISSIONS OWNER_READ OWNER_WRITE
        COMPONENT fashio_mnist
    )
endif()