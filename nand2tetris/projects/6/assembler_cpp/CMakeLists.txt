cmake_minimum_required(VERSION 3.29.0)
set(project_name "HACK-Assembler-XYZ")
project(${project_name})

set(SOURCE_FILES
"main.cpp"
"SymbolTable.cpp"
"Parser.cpp"
)

add_executable(${project_name} ${SOURCE_FILES})

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pass C++20 standard flag
target_compile_options(${project_name} PRIVATE -std=c++20)
