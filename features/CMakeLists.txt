set(CucumberCpp_ROOT "/home/kith/apps/cucumber")
find_package(CucumberCpp REQUIRED)
find_package(Boost COMPONENTS thread program_options regex filesystem unit_test_framework REQUIRED)
find_package(Threads REQUIRED)

add_executable(runner step_definitions/runner.cpp)

target_include_directories(runner PRIVATE
    ${CucumberCpp_INCLUDE_DIR}
)

target_link_libraries(runner PRIVATE
    ${CucumberCpp_LIBRARY}
    Boost::unit_test_framework
    Boost::filesystem
    Boost::regex
    Boost::program_options
    Boost::thread
    Threads::Threads
)

configure_file(runner.sh runner.sh COPYONLY)

add_test(NAME tuple COMMAND ${CMAKE_BINARY_DIR}/features/runner.sh ${CMAKE_BINARY_DIR}/features/runner ${CMAKE_SOURCE_DIR}/features/tuples.feature)