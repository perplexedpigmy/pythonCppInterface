cmake_minimum_required(VERSION 3.10)
project(${this} C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(this epc)

find_package(Python3 COMPONENTS Development)

#set(CMAKE_VERBOSE_MAKEFILE on) #Uncomment to print each command before it is executed by the generated makefile.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${_Python3_INCLUDE_DIR} -O0 -Wall -Wextra --pedantic -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_EXPORT_COMPILE_COMMANDS true)
set (CMAKE_BUILD_TYPE Debug)

file(GLOB SRC_FILES
    "inc/*.h"
    "src/*.cpp"
)

add_executable(${this} ${SRC_FILES} )
target_link_libraries(${this} ${_Python3_LIBRARY_RELEASE} )
