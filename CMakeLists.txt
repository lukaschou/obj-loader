cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ObjLoader)

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_sources(${PROJECT_NAME}
    PRIVATE
        loader.cpp        

    PUBLIC
    FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/loader.h
)
