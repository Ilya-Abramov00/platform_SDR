cmake_minimum_required(VERSION 3.5)

project(platform_SDR LANGUAGES CXX)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -g -O0")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC include/${PROJECT_NAME}/*.h source/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC RtlSdr )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include("unittest.cmake")
