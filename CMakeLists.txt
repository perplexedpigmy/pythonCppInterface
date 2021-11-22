cmake_minimum_required(VERSION 3.10)
set(PYTHON "/usr/bin/python3.6-config")
set(this epc)

project(${this} C CXX)

#Uncomment to print each command before it is executed by the generated makefile.
set(CMAKE_VERBOSE_MAKEFILE on)

#Derive the compile and link commands for embedded Python
execute_process(COMMAND ${PYTHON} --cflags OUTPUT_VARIABLE PYTHON_C_FLAGS)
#Removes (strips) the newline char at the end of the generated switches.
string(STRIP ${PYTHON_C_FLAGS} PYTHON_C_FLAGS)
execute_process(COMMAND ${PYTHON} --ldflags OUTPUT_VARIABLE PYTHON_LINK_LIBS)
string(STRIP ${PYTHON_LINK_LIBS} PYTHON_LINK_LIBS)

set(CMAKE_CXX_STANDARD 17)  
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PYTHON_C_FLAGS} -O0 -Wall -Wextra --pedantic -fPIC") 
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

#Build rules
add_executable(${this} ${SRC_FILES} )
target_link_libraries(${this}  ${PYTHON_LINK_LIBS})

