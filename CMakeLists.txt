cmake_minimum_required(VERSION 3.19.2)

project(Program)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/include)

set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)

set(PROJECT_SOURCES
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/ClientData.cpp
        ${PROJECT_SOURCE_DIR}/primary.cpp
        ${PROJECT_SOURCE_DIR}/secondary.cpp
    )

set(PROJECT_HEADERS
		${PROJECT_INCLUDE_DIR}/ClientData.hpp
        ${PROJECT_INCLUDE_DIR}/Primary.hpp
		${PROJECT_INCLUDE_DIR}/Secondary.hpp

	)

include_directories(${PROJECT_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})