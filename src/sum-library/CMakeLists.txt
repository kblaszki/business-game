add_library(sumLibrary SHARED LibraryCode.cpp)
target_compile_options(sumLibrary PRIVATE ${BASE_COMPILE_FLAGS})