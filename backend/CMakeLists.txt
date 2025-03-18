cmake_minimum_required(VERSION 3.10)
set(project_name "GoGame")
project(${project_name})

set(CMAKE_CXX_STANDARD 17)

include_directories(server/include backend/include lib)

file(GLOB_RECURSE SERVER_SRC server/src/*.cpp)
file(GLOB_RECURSE BACKEND_SRC backend/src/*.cpp)

add_executable(backend ${SERVER_SRC} ${BACKEND_SRC})
