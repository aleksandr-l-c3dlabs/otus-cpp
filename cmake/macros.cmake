# Задача
MACRO(_task target useTemplates)
    add_executable(${target} main.cpp)
    add_library(${target}_lib lib.cpp ${ARGN})
    add_library(${target}_i INTERFACE)

    set_target_properties(${target} ${target}_lib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    target_include_directories(${target}_lib
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    )

    if(${useTemplates})
        target_link_libraries(${target}
            PRIVATE ${target}_i
        )
    else()
        target_link_libraries(${target}
            PRIVATE ${target}_lib
        )
    endif()

    target_include_directories(${target}_i INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")
    target_compile_features(${target}_i INTERFACE cxx_std_20)

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
    _test_src(${target} ${sources})
    _test_link(${target} ${ARGN})
ENDMACRO()


#
MACRO(_test_src target)
    add_executable(${target} ${ARGN})

    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_include_directories(${target}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
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


#
MACRO(_test_link target)
    target_link_libraries(${target}
        PRIVATE 
        ${ARGN}
        GTest::gtest_main
    )
ENDMACRO()



