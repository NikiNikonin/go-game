cmake_minimum_required(VERSION 3.10)
set(project_name "GoGame")
set(exeutable_name "backend")
project(${project_name})

set(CMAKE_CXX_STANDARD 17)

set(LIBTORCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/libtorch)
set(CMAKE_PREFIX_PATH ${LIBTORCH_DIR})
set(ATEN_NO_CUDA 1)
set(BUILD_CUSTOM_PROTOBUF OFF)

find_package(Torch REQUIRED PATHS ${LIBTORCH_DIR} NO_DEFAULT_PATH)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

include_directories(server/include backend/include lib)

file(GLOB_RECURSE SERVER_SRC server/src/*.cpp)
file(GLOB_RECURSE GAME_SRC game/src/*.cpp)

add_executable(${exeutable_name} ${SERVER_SRC} ${GAME_SRC})
target_link_libraries(${exeutable_name} "${TORCH_LIBRARIES}")
set_property(TARGET ${exeutable_name} PROPERTY CXX_STANDARD 17)
