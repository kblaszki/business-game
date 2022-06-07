cmake_minimum_required(VERSION 3.3)

project(business-game)

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

add_library(sumLibrary STATIC LibraryCode.cpp)

add_executable(mainApp main.cpp)
target_link_libraries(mainApp sumLibrary)

add_executable(testCommon test.cpp)
target_link_libraries(testCommon sumLibrary gtest_main)

add_test(unitTest testCommon)