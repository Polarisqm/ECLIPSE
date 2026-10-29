cmake_minimum_required(VERSION 3.20)

project(ECLIPSE
    VERSION 0.2.0
    DESCRIPTION "Artificial Life & Emergent Intelligence Simulator"
    LANGUAGES CXX
)

add_executable(eclipse
    src/main.cpp
    src/world.cpp
)

target_include_directories(eclipse PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(eclipse PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(eclipse PRIVATE /W4 /permissive-)
else()
    target_compile_options(eclipse PRIVATE -Wall -Wextra -Wpedantic)
endif()
