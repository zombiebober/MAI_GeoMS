cmake_minimum_required(VERSION 2.8)
project(conan)
add_definitions("-std=c++11")

#    message(STATUS conan_check_compiler())

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


add_executable(conan main.cpp Weather.cpp Weather.h WeatherForecast.h WeatherForecast.cpp)

target_link_libraries(conan ${CONAN_LIBS})