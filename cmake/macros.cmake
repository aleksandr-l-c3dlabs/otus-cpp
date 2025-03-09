# Задача
MACRO(_task target)
    add_executable(${target} main.cpp)
    add_library(${target}_lib lib.cpp)

    set_target_properties(${target} ${target}_lib PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_include_directories(${target}_lib
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    )

    target_link_libraries(${target}
        PRIVATE ${target}_lib
    )

    if (MSVC)
        target_compile_options(${target} PRIVATE
            /W4
        )
        target_compile_options(${target}_lib PRIVATE
            /W4
        )
    else ()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
        target_compile_options(${target}_lib PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()

    install(TARGETS ${target} RUNTIME DESTINATION bin)
ENDMACRO()


# Добавить новый тест в сборку
MACRO(_test target sources)
    add_executable(${target} ${sources})

    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_include_directories(${target}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    )

    target_link_libraries(${target}
        ${ARGN}
        GTest::gtest_main
    )

    if (MSVC)
        target_compile_options(${target} PRIVATE
            /W4
        )
    else ()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()

    add_test(NAME ${target} COMMAND ${target})
ENDMACRO()



