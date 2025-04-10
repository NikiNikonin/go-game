cmake_minimum_required(VERSION 3.10)
set(project_name "GoGame")
set(exeutable_name "backend")
project(${project_name})

set(CMAKE_CXX_STANDARD 17)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

include_directories(server/include backend/include lib)

file(GLOB_RECURSE SERVER_SRC server/src/*.cpp)
file(GLOB_RECURSE GAME_SRC game/src/*.cpp)

add_executable(${exeutable_name} ${SERVER_SRC} ${GAME_SRC})