set(_cucumberCpp_HEADER_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include"
    "${CMAKE_SOURCE_DIR}/includes"
    "${CMAKE_SOURCE_DIR}/build-cucumber-cpp/dist/include"
)

set(_cucumberCpp_LIB_SEARCH_DIRS
    "/usr/lib"
    "/usr/local/lib"
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_SOURCE_DIR}/build-cucumber-cpp/dist/lib"
)

# check env
set(_cucumberCpp_ENV_ROOT $ENV{CucumberCpp_ROOT})
if(NOT CucumberCpp_ROOT AND _cucumberCpp_ENV_ROOT)
    set(CucumberCpp_ROOT ${_cucumberCpp_ENV_ROOT})
endif()

# search user specified location first
if(CucumberCpp_ROOT)
    list(INSERT _cucumberCpp_HEADER_SEARCH_DIRS @ "${CucumberCpp_ROOT}/include")
    list(INSERT _cucumberCpp_LIB_SEARCH_DIRS @ "${CucumberCpp_ROOT}/lib")
endif()

# search for headers
FIND_PATH(CucumberCpp_INCLUDE_DIR "cucumber-cpp/autodetect.hpp" PATHS ${_cucumberCpp_HEADER_SEARCH_DIRS})

# search for libs
FIND_LIBRARY(CucumberCpp_LIBRARY NAMES cucumber-cpp PATHS ${_cucumberCpp_LIB_SEARCH_DIRS})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CucumberCpp DEFAULT_MSG CucumberCpp_LIBRARY CucumberCpp_INCLUDE_DIR)