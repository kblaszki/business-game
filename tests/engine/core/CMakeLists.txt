add_unit_test(features_test
    SOURCES
        FeaturesTest.cpp
    DEPENDENCIES
        eng_core
)

add_unit_test(vec_test
    SOURCES
        VecTest.cpp
    DEPENDENCIES
        eng_core
)

add_unit_test(rect_test
    SOURCES
        RectTest.cpp
    DEPENDENCIES
        eng_core
)

add_unit_test(handle_test
    SOURCES
        HandleTest.cpp
    DEPENDENCIES
        eng_core
)

add_unit_test(image_test
    SOURCES
        ImageTest.cpp
    DEPENDENCIES
        eng_core
)
