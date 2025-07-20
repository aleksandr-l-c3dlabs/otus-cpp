cmake_minimum_required(VERSION 3.28)

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


_get_cpack_components(components)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
set(CPACK_DEB_COMPONENT_INSTALL ON)
foreach(comp IN LISTS components)
    string(TOUPPER ${comp} upper_comp)
    set(CPACK_DEBIAN_${upper_comp}_PACKAGE_NAME "${comp}")
    set(CPACK_DEBIAN_${upper_comp}_FILE_NAME "${comp}.deb")
    set(CPACK_DEBIAN_${upper_comp}_PACKAGE_SHLIBDEPS ON)
endforeach()
include(CPack)
foreach(comp IN LISTS components)
    cpack_add_component_group("${comp}")

    cpack_add_component("${comp}"
                        DISPLAY_NAME "${comp}"
                        GROUP "${comp}"
                        INSTALL_TYPES Full)
endforeach()