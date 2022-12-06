include(FetchContent)

FetchContent_Declare(sfml
        GIT_REPOSITORY git@github.com:SFML/SFML.git
        GIT_TAG 2.5.1)
FetchContent_GetProperties(sfml)
if (NOT sfml_POPULATED)
    FetchContent_Populate(sfml)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()