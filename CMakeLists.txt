cmake_minimum_required(VERSION 3.3)

project(business-game)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CURRENT_BINARY_DIR ${EXECUTABLE_OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH  ${LIBRARY_OUTPUT_PATH}/lib)

#get googletest
include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY git@github.com:google/googletest.git
        GIT_TAG release-1.11.0)
FetchContent_GetProperties(googletest)
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
if(NOT sfml_POPULATED)
    FetchContent_Populate(sfml)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BUILD_DIR})
endif()
include_directories(${sfml_SOURCE_DIR}/include)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_TYPE debug)
    set(BUILD_VERSION -d-2)
else()
    set(BUILD_TYPE release)
    set(BUILD_VERSION -2)
endif()
set(SFML_DLL
    ${CMAKE_BINARY_DIR}/build/${BUILD_TYPE}/_deps/sfml-src/lib/sfml-graphics${BUILD_VERSION}.dll
    ${CMAKE_BINARY_DIR}/build/${BUILD_TYPE}/_deps/sfml-src/lib/sfml-window${BUILD_VERSION}.dll
    ${CMAKE_BINARY_DIR}/build/${BUILD_TYPE}/_deps/sfml-src/lib/sfml-system${BUILD_VERSION}.dll
)

add_library(sumLibrary STATIC LibraryCode.cpp)

add_executable(mainApp main.cpp)
target_link_libraries(mainApp
        sfml-graphics
        sfml-system
        sfml-window)
add_custom_command(TARGET mainApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${SFML_DLL}
        ${EXECUTABLE_OUTPUT_PATH}
        COMMENT "Copying ${SFML_DLL} library to '${EXECUTABLE_OUTPUT_PATH}'")


add_executable(testCommon test.cpp)
target_link_libraries(testCommon sumLibrary gtest_main)

add_test(unitTest testCommon)