cmake_minimum_required(VERSION 3.14)
project(appstream_sax)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LibXml2 REQUIRED)

add_executable(${PROJECT_NAME}
        AppStreamParser.cpp
        Component.cpp
        AppStreamParser.h
        Component.h
        main.cpp
)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.0
)

FetchContent_MakeAvailable(spdlog)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog LibXml2::LibXml2)