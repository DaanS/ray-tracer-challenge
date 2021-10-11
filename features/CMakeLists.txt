find_package(CucumberCpp REQUIRED)
find_package(Boost COMPONENTS filesystem program_options regex thread unit_test_framework REQUIRED)
find_package(Threads REQUIRED)

#include(FetchContent)
#FetchContent_Declare(google_test
#    GIT_REPOSITORY https://github.com/google/googletest
#    GIT_TAG HEAD
#)
#FetchContent_MakeAvailable(google_test)

add_executable(runner step_definitions/runner.cpp)

target_include_directories(runner PRIVATE
    ${CucumberCpp_INCLUDE_DIR}
    ${google_test_SOURCE_DIR}/googletest/include
)

target_link_libraries(runner PRIVATE
    ${CucumberCpp_LIBRARY}
    Boost::filesystem
    Boost::program_options
    Boost::regex
    Boost::thread
    Boost::unit_test_framework
    Threads::Threads
)

configure_file(runner.sh runner.sh COPYONLY)

add_test(NAME tuple COMMAND ${CMAKE_BINARY_DIR}/features/runner.sh ${CMAKE_BINARY_DIR}/features/runner ${CMAKE_SOURCE_DIR}/features/tuples.feature)