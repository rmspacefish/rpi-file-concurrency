cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED OS_APP)
    set(OS_APP "host")
endif()

set(OS_APP ${OS_APP} CACHE STRING "Application OS")

set_property(CACHE OS_APP PROPERTY STRINGS "host" "rpi")

if(OS_APP MATCHES "rpi")
    set(CMAKE_TOOLCHAIN_FILE "RPiToolchain.cmake")
endif()

# set the project name and version
project(fs-concurrency LANGUAGES CXX C VERSION 1.0)
set(TARGET_NAME ${CMAKE_PROJECT_NAME})

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE 
    main.cpp
    read.cpp
    write.cpp
)

target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads)

