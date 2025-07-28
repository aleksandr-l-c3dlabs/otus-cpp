add_subdirectory(shared)

_task(async OFF async)

target_link_libraries(async PRIVATE 
    async_shared
)

