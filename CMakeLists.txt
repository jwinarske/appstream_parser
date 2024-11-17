cmake_minimum_required(VERSION 3.14)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release, RelWithDebInfo, or MinSizeRel." FORCE)
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to Release.")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(appstream_parser
        VERSION 0.0.1
        DESCRIPTION "Appstream Parser"
        LANGUAGES CXX
)

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

#
# Packaging
#
if (NOT CMAKE_CROSSCOMPILING)
    include(packaging)
endif ()
