cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDART 20)

project(weatherFrts)

set(CMAKE_GENERATOR "MinGW Makefiles")


if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8) 
FetchContent_MakeAvailable(cpr)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz) 
FetchContent_MakeAvailable(json)

add_subdirectory(bin)