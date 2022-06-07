cmake_minimum_required(VERSION 3.3)

project(business-game)

#set(CMAKE_CXX_STANDARD 14)

#get googletest
include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY git@github.com:google/googletest.git
        GIT_TAG release-1.11.0)
FetchContent_GetProperties(googletest)
#googletest_POPULATED
#googletest_SOURCE_DIR
#googletest_BUILD_DIR
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

#get sfml
include(FetchContent)
FetchContent_Declare(sfml
        GIT_REPOSITORY git@github.com:SFML/SFML.git
        GIT_TAG 2.5.1)
FetchContent_GetProperties(sfml)
#sfml_POPULATED
#sfml_SOURCE_DIR
#sfml_BUILD_DIR
if(NOT sfml_POPULATED)
    FetchContent_Populate(sfml)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BUILD_DIR})
endif()
include_directories(${sfml_SOURCE_DIR}/include)

add_library(sumLibrary STATIC LibraryCode.cpp)

add_executable(mainApp main.cpp)
target_link_libraries(mainApp
        sumLibrary
        sfml-graphics
        sfml-system
        sfml-window)

add_executable(testCommon test.cpp)
target_link_libraries(testCommon sumLibrary gtest_main)

add_test(unitTest testCommon)

