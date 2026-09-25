include(FetchContent)

set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

# CMake 4 rejects cmake_minimum_required() below 3.5. Vorbis still declares 2.8.12.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

FetchContent_Declare(sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0)
FetchContent_MakeAvailable(sfml)

# SFML silences codec warnings with GCC's -w. MSVC rejects that flag.
if(MSVC)
    foreach(codec IN ITEMS ogg FLAC vorbis vorbisenc vorbisfile)
        if(TARGET ${codec})
            get_target_property(codec_options ${codec} COMPILE_OPTIONS)
            if(codec_options)
                list(REMOVE_ITEM codec_options "-w")
                set_property(TARGET ${codec} PROPERTY COMPILE_OPTIONS ${codec_options})
            endif()
        endif()
    endforeach()
endif()
