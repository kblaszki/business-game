include(FetchContent)

set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

FetchContent_Declare(sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0)
FetchContent_MakeAvailable(sfml)
