cmake_minimum_required(VERSION 3.10)

project(metaprogram VERSION 0.0.1)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Threads REQUIRED)

add_executable(metaprogram main.cpp)

add_executable(tests tests.cpp)

set_target_properties(metaprogram tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

set_target_properties(tests PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}"
)

target_link_libraries(metaprogram
    Threads::Threads
)

target_link_libraries(tests
    ${Boost_LIBRARIES}
)

install(TARGETS metaprogram RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT baranovsky.s.a@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(utest tests)

