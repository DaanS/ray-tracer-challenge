include(FetchContent)
FetchContent_Declare(google_test
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG HEAD
)
FetchContent_MakeAvailable(google_test)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(tuples
    tuples.cpp
)
target_link_libraries(tuples
    gtest_main
)

add_executable(canvas
    canvas.cpp
)
target_link_libraries(canvas
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tuples)
gtest_discover_tests(canvas)