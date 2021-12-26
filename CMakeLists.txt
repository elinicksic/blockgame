cmake_minimum_required(VERSION 3.10)
project(blockgame LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(EXT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/src")
set(EXT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/include")
add_library(glad "${EXT_INCLUDE_DIR}/glad/glad.h" "${EXT_SOURCE_DIR}/glad.c")
target_include_directories(glad PUBLIC ${EXT_INCLUDE_DIR})

add_executable(${PROJECT_NAME} 
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/shader.cpp"
    "${CMAKE_SOURCE_DIR}/src/camera.cpp"
    "${CMAKE_SOURCE_DIR}/src/input.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad glm PRIVATE ${CMAKE_DL_LIBS})