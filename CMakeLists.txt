cmake_minimum_required(VERSION 3.20)

project(business-game VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND
        BASE_COMPILE_FLAGS
        "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall;-Wextra;-pedantic;-Werror>"
        "$<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>")

include(FetchSFML)

include_directories(src)
add_subdirectory(src)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND
            BASE_COMPILE_FLAGS
            "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-O3>"
            "$<$<CXX_COMPILER_ID:MSVC>:/Ox>")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND
            BASE_COMPILE_FLAGS
           "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-g>"
            "$<$<CXX_COMPILER_ID:MSVC>:/Zi>")

    file(GLOB_RECURSE FORMAT_SOURCES "src/*.cpp" "src/*.hpp" "tests/*.cpp" "tests/*.hpp")
    
    add_custom_target(
        format
        COMMAND clang-format -i ${FORMAT_SOURCES}
    )
    
    include(CTest)
    add_subdirectory(tests EXCLUDE_FROM_ALL)
endif()
