cmake_minimum_required(VERSION 3.0.0)

project(skyscrappers)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(testapp test.cpp)
target_link_libraries(testapp ${CONAN_LIBS})
set_property(TARGET testapp PROPERTY CXX_STANDARD 11)

