include(FetchContent)
FetchContent_Declare(google_test
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG HEAD
)
FetchContent_MakeAvailable(google_test)

include_directories(${CMAKE_SOURCE_DIR}/src)
include(GoogleTest)

add_executable(tuples tuples.cpp)
target_link_libraries(tuples gtest_main)
gtest_discover_tests(tuples)

add_executable(canvas canvas.cpp)
target_link_libraries(canvas gtest_main)
gtest_discover_tests(canvas)

add_executable(matrix matrix.cpp)
target_link_libraries(matrix gtest_main)
gtest_discover_tests(matrix)

add_executable(transform transform.cpp)
target_link_libraries(transform gtest_main)
gtest_discover_tests(transform)

add_executable(ray ray.cpp)
target_link_libraries(ray gtest_main)
gtest_discover_tests(ray)

add_executable(sphere sphere.cpp)
target_link_libraries(sphere gtest_main)
gtest_discover_tests(sphere)

add_executable(intersection intersection.cpp)
target_link_libraries(intersection gtest_main)
gtest_discover_tests(intersection)

add_executable(light light.cpp)
target_link_libraries(light gtest_main)
gtest_discover_tests(light)

add_executable(material material.cpp)
target_link_libraries(material gtest_main)
gtest_discover_tests(material)

add_executable(world world.cpp)
target_link_libraries(world gtest_main)
gtest_discover_tests(world)

add_executable(camera camera.cpp)
target_link_libraries(camera gtest_main)
gtest_discover_tests(camera)