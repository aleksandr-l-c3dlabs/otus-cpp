cmake_minimum_required(VERSION 3.14)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(otus-homeworks VERSION ${PROJECT_VERSION})

include(cmake/macros.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(WITH_TEST "Whether to build test" ON)

add_subdirectory(src)

if(WITH_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()


set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
