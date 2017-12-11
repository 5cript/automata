# Version check
cmake_minimum_required (VERSION 3.0)

# Project
project(automata)

# Add files
file(GLOB sources "*.cpp")

# Add library
add_library(automata STATIC ${sources})

# Compiler Options
target_compile_options(automata PRIVATE -fexceptions -std=c++14 -O3 -Wall -pedantic-errors -pedantic)