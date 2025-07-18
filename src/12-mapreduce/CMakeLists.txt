add_subdirectory(mapper_column)
add_subdirectory(reducer_avg)
add_subdirectory(reducer_var)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(
        FILES "./sh/run_avg.sh"
        DESTINATION "/usr/bin"
        PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
        COMPONENT mapreduce
    )

    install(
        FILES "./sh/run_var.sh"
        DESTINATION "/usr/bin"
        PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
        COMPONENT mapreduce
    )

    install(
        FILES "./preset/AB_NYC_2019.csv"
        DESTINATION "/var/lib/mapreduce"
        PERMISSIONS OWNER_READ OWNER_WRITE
        COMPONENT mapreduce
    )
endif()

