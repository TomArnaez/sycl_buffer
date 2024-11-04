set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl -fsycl-targets=native_cpu")

project(test_lib LANGUAGES CXX)

add_library(test_lib SHARED src/lib.cpp)
target_include_directories(test_lib PUBLIC include/)
target_compile_features(test_lib PRIVATE cxx_std_20)